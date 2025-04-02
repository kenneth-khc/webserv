/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:22:41 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <iostream>
#include "Optional.hpp"
#include "String.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Configuration.hpp"
#include "Configurator.hpp"
#include "ConfigErrors.hpp"
#include "Logger.hpp"

Parser::Parser(const char *fileName):
	configurator(),
	lexer(fileName),
	token(Token::NONE),
	contexts(),
	accepted()
{
}

Parser::~Parser()
{

}

Configuration	Parser::parseConfig()
{
	Configuration	config;

	contexts.push(GLOBAL);
	// TODO: testing stack of multimaps
	mapStack.push(std::multimap<String,Directive*>());
	parents.push(NULL);
	try
	{
		token = lexer.advance();
		while (token != Token::END_OF_FILE)
		{
			Directive*	directive = parseDirective();
			configurator.add(directive, config);
		}
		config.assertHasDirective("prefix");
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
	const String	name = token.lexeme;
	expect(Token::NAME);

	Diagnostic	diagnostics = Diagnostic(accepted.lineNum, accepted.columnNum);
	// TODO: is there a way to strengthen a Parameter rather than
	//		 treating it as a vector of Strings?
	std::vector<Parameter>	parameters = parseParameters();

	Directive*	directive;
	if (token == Token::LCURLY)
	{
		directive = parseBlock(name, parameters);
	}
	else if (token == Token::SEMICOLON)
	{
		directive = parseSimple(name, parameters);
		directive->enclosing = mapStack.top();
		directive->parent = parents.top();
	}
	else
	{
		throw UnexpectedToken(token);
	}
	directive->diagnostic = diagnostics;
	// validate the directive in the context of its surrounding
	configurator.validate(directive, mapStack.top());
	std::pair<String,Directive*>	mapping = std::make_pair(directive->name, directive);
	mapStack.top().insert(mapping);
	return directive;
}

std::vector<Parameter>	Parser::parseParameters()
{
	std::vector<Parameter>	params;

	while (token.type == Token::PARAMETER)
	{
		Parameter	p = Parameter(token.lexeme, token.diagnostic);
		accept(Token::PARAMETER);
		params.push_back(p);
	}
	lexer.lookFor(Token::NAME);
	return params;
}

Directive*	Parser::parseSimple(const String& name,
								const std::vector<Parameter>& params)
{
	expect(Token::SEMICOLON);
	return new Directive(name, params, contexts.top());
}

Directive*	Parser::parseBlock(const String& name,
							   const std::vector<Parameter>& params)
{
	expect(Token::LCURLY);
	Directive*	block = new Directive(name, params, contexts.top());
	contexts.push(contextify(name));
	block->parent = parents.top();
	parents.push(block);
	mapStack.push(std::multimap<String,Directive*>());
	while (token != Token::RCURLY)
	{
		Directive* directive = parseDirective();
		block->addDirective(directive);
	}
	expect(Token::RCURLY);
	contexts.pop();
	mapStack.pop();
	parents.pop();
	block->enclosing = mapStack.top();
	return block;
}

void	Parser::expect(Token::TokenType expected)
{
	if (accept(expected))
	{
		return ;
	}
	else
	{
		throw UnexpectedToken(expected, token);
		throw UnexpectedToken(expected, token.type);
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
