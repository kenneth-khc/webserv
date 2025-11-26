/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/04 22:52:11 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "String.hpp"
#include "Configuration.hpp"
#include "Validators.hpp"
#include "Parameter.hpp"
#include "UnexpectedToken.hpp"
#include "MissingGlobalDirective.hpp"
#include "InvalidContext.hpp"
#include "VectorInitializer.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

Parser::Parser(const char *filename):
	config(new Configuration),
	validators(),
	lexer(filename),
	token(),
	accepted(),
	filename(filename)
{
}

Parser::~Parser()
{
	delete config;
	config = NULL;
}

Configuration*	Parser::parseConfig()
{
	token = lexer.advance();
	while (token != Token::END_OF_FILE)
	{
		parseDirective();
	}
	if (!config->getDirective("prefix").exists)
	{
		throw MissingGlobalDirective(filename, "prefix");
	}
	if (!config->getDirective("http").exists)
	{
		throw MissingGlobalDirective(filename, "http");
	}
	Configuration*	completeConfig = config;
	config = NULL;
	return completeConfig;
}

void	Parser::parseDirective()
{
	const String		name = token.lexeme;
	const Diagnostic	diagnostics = token.diagnostic;
	const Token			nameToken = token;

	expect(Token::NAME);

	std::vector<Parameter>	parameters = parseParameters();

	if (token == Token::LCURLY)
	{
		parseBlock(name, parameters, diagnostics);
	}
	else if (token == Token::SEMICOLON)
	{
		parseSimple(name, parameters, diagnostics);
	}
	else
	{
		throw UnexpectedToken(token, nameToken, vector_of(Token(Token::LCURLY, "{"))(Token(Token::SEMICOLON, ";")));
	}
}


std::vector<Parameter>	Parser::parseParameters()
{
	std::vector<Parameter>	parameters;

	while (token.type == Token::PARAMETER || token.type == Token::NEWLINE)
	{
		if (token.type == Token::NEWLINE)
		{
			accept(Token::NEWLINE);
		}
		else
		{
			Parameter	parameter = Parameter(token.lexeme, token.diagnostic);
			accept(Token::PARAMETER);
			parameters.push_back(parameter);
		}
	}
	lexer.lookFor(Token::NAME);

	return parameters;
}

void	Parser::parseSimple(const String& name,
								const std::vector<Parameter>& parameters,
								const Diagnostic& diagnostic)
{
	expect(Token::SEMICOLON);

	Directive*	parent = parents.empty() ? NULL : parents.top();
	Directive*	directive = new Directive(name, parameters, parent, diagnostic);
	if (parent)
	{
		parent->addDirective(directive);
	}
	else
	{
		config->add(directive);
	}
	validators.validate(directive,
						directive->parent ?
						directive->parent->getDirectives() :
						config->directives);

}

void	Parser::parseBlock(const String& name,
							   const std::vector<Parameter>& params,
							   const Diagnostic& diagnostic)
{
	expect(Token::LCURLY);

	Directive*	parent = parents.size() == 0 ? NULL : parents.top();
	Directive*	block = new Directive(name, params, parent, diagnostic);
	if (parent)
	{
		parent->addDirective(block);
	}
	else
	{
		config->add(block);
	}
	const Validator&	validator = validators.getValidator(block);
	try
	{
		validator.validateHeader(block, block->parent ?
										block->parent->getDirectives() :
										config->directives);
	}
	catch (const std::runtime_error&)
	{
		throw InvalidContext(*block);
	}
	parents.push(block);
	while (token != Token::RCURLY)
	{
		parseDirective();
	}
	expect(Token::RCURLY);
	parents.pop();
	validator.validateBody(block, block->parent ?
								  block->parent->getDirectives() :
								  config->directives);
}

void	Parser::expect(Token::TokenType expected)
{
	static std::vector<Token>	expectedTokens = vector_of<Token>
		(Token(Token::NONE, "None"))
		(Token(Token::NAME, "Name"))
		(Token(Token::PARAMETER, "Parameter"))
		(Token(Token::DQUOTE, "\""))
		(Token(Token::LCURLY, "{"))
		(Token(Token::RCURLY, "}"))
		(Token(Token::SEMICOLON, ";"))
		(Token(Token::HASH, "#"))
		(Token(Token::NEWLINE, "\n"))
		(Token(Token::END_OF_FILE, "EOF"));

	Token	prev = accepted;
	if (accept(expected))
	{
		return ;
	}
	else
	{
		throw UnexpectedToken(token, prev, expectedTokens[expected]);
	}
}

bool	Parser::accept(Token::TokenType type)
{
	if (token.type == type)
	{
		accepted = token;
		if (type == Token::NAME)
		{
			lexer.lookFor(Token::PARAMETER);
		}
		token = lexer.advance();
		return true;
	}
	else
	{
		return false;
	}
}
