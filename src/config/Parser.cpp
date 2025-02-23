/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/22 01:45:09 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
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
}

Configuration	Parser::parseConfig()
{
	Configuration	config;

	contexts.push(GLOBAL);
	try
	{
		token = lexer.advance();
		while (token != Token::END_OF_FILE)
		{
			Directive	directive = parseDirective();
			config.add(directive);
		}
	}
	catch (const ConfigError& e)
	{
		std::cerr << e.what() << '\n';
	}
	return config;
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
	configurator.validate(directive);
	return directive;
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
	while (token != Token::RCURLY)
	{
		Directive directive = parseDirective();
		configurator.validate(directive);
		block.addDirective(directive);
	}
	expect(Token::RCURLY);
	contexts.pop();
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
