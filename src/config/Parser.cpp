/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/02 03:19:01 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include "String.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Configuration.hpp"
#include "Configurator.hpp"
#include "ConfigErrors.hpp"

Parser::Parser(const char *fileName):
configFile(fileName),
configurator(),
lexer(fileName),
token(Token::NONE),
contexts()
{
	if (!configFile.is_open())
	{
		std::cerr << "Error. Cannot open file <" << fileName << ">\n";
		std::exit(1);
	}
}

Configuration	Parser::parseConfig()
{
	Configuration	config;

	contexts.push(GLOBAL);
	// TODO: testing stack of multimaps
	mapStack.push(std::multimap<String,Directive>());
	try
	{
		token = lexer.advance();
		while (token != Token::END_OF_FILE)
		{
			Directive	directive = parseDirective();
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

Directive	Parser::parseDirective()
{
	const String	name = token.lexeme;
	expect(Token::NAME);

	// TODO: is there a way to strengthen a Parameter rather than
	//		 treating it as a vector of Strings?
	std::vector<String>	parameters = parseParameters();

	Directive	directive;
	if (token == Token::LCURLY)
	{
		directive = parseBlock(name, parameters);
	}
	else if (token == Token::SEMICOLON)
	{
		directive = parseSimple(name, parameters);
	}
	else
	{
		throw UnexpectedToken(token);
	}
	configurator.validate(directive, mapStack.top());
	std::pair<String,Directive>	mapping = std::make_pair(directive.name, directive);
	mapStack.top().insert(mapping);
	return directive;
}

std::vector<String>	Parser::parseParameters()
{
	std::vector<String>	parameters;

	while (token.type == Token::PARAMETER)
	{
		const String&	param = token.lexeme;
		parameters.push_back(param);
		accept(Token::PARAMETER);
	}
	lexer.lookingFor = Token::NAME;
	return parameters;
}

Directive	Parser::parseSimple(const String& name,
								const std::vector<String>& params)
{
	expect(Token::SEMICOLON);
	return Directive(name, params, contexts.top());
}

Directive	Parser::parseBlock(const String& name,
							   const std::vector<String>& params)
{
	expect(Token::LCURLY);
	Directive	block = Directive(name, params, contexts.top());
	contexts.push(contextify(name));
	mapStack.push(std::multimap<String,Directive>());
	while (token != Token::RCURLY)
	{
		Directive directive = parseDirective();
		block.addDirective(directive);
	}
	expect(Token::RCURLY);
	contexts.pop();
	mapStack.pop();
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
		throw UnexpectedToken(expected, token.type);
	}
}

bool	Parser::accept(Token::TokenType type)
{
	if (token.type == type)
	{
		if (type == Token::NAME)
		{
			lexer.lookingFor = Token::PARAMETER;
		}
		token = lexer.advance();
		return true;
	}
	else
	{
		return false;
	}
}
