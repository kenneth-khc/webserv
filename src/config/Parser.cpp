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
#include "Configurator.hpp"
#include "Parameter.hpp"
#include "UnexpectedToken.hpp"
#include "MissingDirective.hpp"
#include "InvalidContext.hpp"
#include "VectorInitializer.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

Parser::Parser(const char *filename):
	config(),
	configurator(),
	lexer(filename),
	token(Token::NONE),
	accepted(),
	filename(filename)
{
}

Parser::~Parser()
{
}

Configuration	Parser::parseConfig()
{
	try
	{
		token = lexer.advance();
		while (token != Token::END_OF_FILE)
		{
			Directive*	directive = parseDirective();
			configurator.add(directive, config);
		}
		if (!config.getDirective("prefix").exists)
		{
			throw MissingDirective(filename, "prefix");
		}
		if (!config.getDirective("http").exists)
		{
			throw MissingDirective(filename, "http");
		}
	}
	catch (const ConfigError& e)
	{
		std::cerr << e.format();
		std::exit(1);
	}
	return config;
}

Directive*	Parser::parseDirective()
{
	Directive*			directive = NULL;
	const String		name = token.lexeme;
	const Diagnostic	diagnostics = token.diagnostic;
	const Token			nameToken = token;

	expect(Token::NAME);

	std::vector<Parameter>	parameters = parseParameters();

	if (token == Token::LCURLY)
	{
		directive = parseBlock(name, parameters, diagnostics);
	}
	else if (token == Token::SEMICOLON)
	{
		directive = parseSimple(name, parameters, diagnostics);
	}
	else
	{
		throw UnexpectedToken(token, nameToken, vector_of(Token(Token::LCURLY, "{"))(Token(Token::SEMICOLON, ";")));
	}

	return directive;
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

Directive*	Parser::parseSimple(const String& name,
								const std::vector<Parameter>& parameters,
								const Diagnostic& diagnostic)
{
	expect(Token::SEMICOLON);

	Directive*	parent = parents.empty() ?
						 NULL :
						 parents.top();
	Context		enclosingContext = parents.empty() ?
								   GLOBAL :
								   contextify(parent->name);
	Directive*	directive = new Directive(name,
										  parameters,
										  parent,
										  enclosingContext,
										  diagnostic);
	configurator.validate(directive,
						  directive->parent ?
						  directive->parent->directives :
						  config.directives);

	return directive;
}

Directive*	Parser::parseBlock(const String& name,
							   const std::vector<Parameter>& params,
							   const Diagnostic& diagnostic)
{
	expect(Token::LCURLY);

	Directive*	parent;
	Context		enclosingContext;

	if (parents.size() == 0)
	{
		parent = NULL;
		enclosingContext = GLOBAL;
	}
	else
	{
		parent = parents.top();
		try
		{
			enclosingContext = contextify(parent->name);
		}
		catch (const std::invalid_argument& e)
		{
			Directive* block = new Directive(name,
											 params,
											 parent,
											 NONE,
											 diagnostic);

			/* TODO(kecheong):
			   there needs to be a way to get the intended blocks
			   for the directive here */
			throw InvalidContext(*block);
		}
	}

	Directive*	block = new Directive(name,
									  params,
									  parent,
									  enclosingContext,
									  diagnostic);
	configurator.validate(block,
						  block->parent ?
						  block->parent->directives :
						  config.directives);
	parents.push(block);
	while (token != Token::RCURLY)
	{
		Directive* directive = parseDirective();
		block->addDirective(directive);
	}
	expect(Token::RCURLY);
	parents.pop();

	return block;
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
