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

#include <cctype>
#include <cstdlib>
#include <iostream>
#include "String.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Configuration.hpp"
#include "Configurator.hpp"
#include "ConfigErrors.hpp"
#include "VectorInitializer.hpp"

Parser::Parser(const char *fileName):
	configurator(),
	lexer(fileName),
	token(Token::NONE),
	accepted()
{
}

Parser::~Parser()
{

}

Configuration	Parser::parseConfig()
{
	Configuration	config;

	// TODO: testing stack of multimaps
	/*mapStack.push(std::multimap<String,Directive*>());*/
	try
	{
		token = lexer.advance();
		while (token != Token::END_OF_FILE)
		{
			Directive*	directive = parseDirective();
			configurator.add(directive, config);
		}
		/*config.assertHasDirective("prefix");*/
		config.assertHasDirective("http");
	}
	// TODO: better diagnostics
	catch (const ConfigError& e)
	{
		std::cerr << e.what() << '\n';
		std::exit(E_CONFIG);
	}
	return config;
}

Directive*	Parser::parseDirective()
{
	Directive*			directive = NULL;
	const String		name = token.lexeme;
	const Diagnostic	diagnostics = token.diagnostic;
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
		throw UnexpectedToken(token,
				vector_of<Token>(Token(Token::LCURLY, "{"))
								(Token(Token::SEMICOLON, ";")));
	}
	directive->diagnostic = diagnostics;
	if (directive->parent)
		configurator.validate(directive, directive->parent->directives);
	// validate the directive in the context of its surrounding
	/*configurator.validate(directive, mapStack.top());*/
	std::pair<String,Directive*>	mapping = std::make_pair(directive->name, directive);
	/*mapStack.top().insert(mapping);*/
	return directive;
}

std::vector<Parameter>	Parser::parseParameters()
{
	std::vector<Parameter>	parameters;
	while (token.type == Token::PARAMETER)
	{
		Parameter	parameter = Parameter(token.lexeme, token.diagnostic);
		accept(Token::PARAMETER);
		parameters.push_back(parameter);
	}
	lexer.lookFor(Token::NAME);
	return parameters;
}

Directive*	Parser::parseSimple(const String& name,
								const std::vector<Parameter>& parameters,
								const Diagnostic& diagnostic)
{
	expect(Token::SEMICOLON);
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
		enclosingContext = contextify(parent->name);
	}
	return new Directive(name, parameters, parent, enclosingContext, diagnostic);
}

Directive*	Parser::parseBlock(const String& name,
							   const std::vector<Parameter>& params,
							   const Diagnostic& diagnostic)
{
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
		enclosingContext = contextify(parent->name);
	}
	Directive*	block = new Directive(name, params, parent, enclosingContext, diagnostic);
	/*Directive*	block = new Directive(name, params, contexts.top());*/
	/*block->diagnostic = generateDiagnostics();*/
	expect(Token::LCURLY);
	/*contexts.push(contextify(name));*/
	/*block->parent = parents.top();*/
	parents.push(block);
	/*mapStack.push(std::multimap<String,Directive*>());*/
	while (token != Token::RCURLY)
	{
		Directive* directive = parseDirective();
		block->addDirective(directive);
	}
	expect(Token::RCURLY);
	/*contexts.pop();*/
	/*mapStack.pop();*/
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

	if (accept(expected))
	{
		return ;
	}
	else
	{
		throw UnexpectedToken(token, expectedTokens[expected]);
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

Diagnostic	Parser::generateDiagnostics() const
{
	return Diagnostic(accepted.lineNum, accepted.columnNum);
}
