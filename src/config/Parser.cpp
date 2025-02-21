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
#include "ConfigValidator.hpp"
#include "ConfigErrors.hpp"
#include "Token.hpp"
#include "Validator.hpp"
#include "Optional.hpp"
#include "Configuration.hpp"

Parser::Parser(const char *fileName):
lexer(fileName),
configFile(fileName)
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
			Optional<Directive>	directive = parseDirective();
			if (directive.exists)
			{
				configValidator.validate(directive.value);
				config.addDirective(directive.value);
			}
		}
	}
	catch (const ConfigError& e)
	{
		std::cerr << e.what() << '\n';
	}
	return config;
}

Optional<Directive>	Parser::parseDirective()
{
	if (token.type == Token::END_OF_FILE)
	{
		return makeNone<Directive>();
	}

	String	name = token.lexeme;
	lexer.lookingFor = Token::PARAMETER;
	expect(Token::NAME);

	if (token == Token::LCURLY)
	{
		lexer.lookingFor = Token::NAME;
		Context	newContext = contextify(name);

		Directive	blockDirective = parseBlock(newContext);
		configValidator.validate(blockDirective);
		return blockDirective;
	}
	// not a {, we are expecting Parameters now
	else
	{
		std::vector<String>	parameters;
		while (token.type == Token::PARAMETER)
		{
			parameters.push_back(token.lexeme);
			accept(Token::PARAMETER);
		}
		lexer.lookingFor = Token::NAME;
		Directive	directive(name, parameters, contexts.top());
		expect(Token::SEMICOLON);
		return directive;
	}
}

Directive	Parser::parseBlock(Context blockName)
{
	// TODO: this is assuming a block directive can't have parameters
	// which isn't true
	Directive	blockDirective(stringifyContext(blockName),
							   std::vector<String>(),
							   contexts.top());
	expect(Token::LCURLY);
	contexts.push(blockName);
	while (token != Token::RCURLY)
	{
		Optional<Directive> directive = parseDirective();
		if (!directive.exists)
		{
			break ;
		}
		configValidator.validate(directive.value);
		blockDirective.addDirective(directive.value);
	}
	expect(Token::RCURLY);
	return blockDirective;
}

std::vector<String>	Parser::parseParameters()
{
	std::vector<String>	parameters;
	Optional<String>	beforeSemicolon = lexer.input.consumeUntil(";");
	if (beforeSemicolon.exists)
	{
		parameters = beforeSemicolon.value.split(" \t\n");
		for (size_t i = 0; i < parameters.size(); ++i)
		{
			std::cout << "Param" << i << ": " << parameters[i] << '\n';
		}
		std::cout << '\n';
	}
	return parameters;
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
		token = lexer.advance();
		return true;
	}
	else
	{
		return false;
	}
}
