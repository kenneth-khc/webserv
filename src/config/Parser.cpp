/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:42:40 by kecheong         ###   ########.fr       */
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

Parser::Parser(const char *fileName):
lexer(fileName),
configFile(fileName)
{

}

void	Parser::parseConfig()
{
	contextStack.push("global");

	try
	{
		token = lexer.advance();
		while (token != Token::END_OF_FILE)
		{
			Optional<Directive>	directive = parseDirective();
			if (directive.exists)
			{
				configValidator.validate(directive.value);
			}
		}
	}
	catch (const ConfigError& e)
	{
		std::cerr << e.what() << '\n';
	}
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
		Directive	blockDirective = parseBlock(name);
		configValidator.validate(blockDirective);
		return blockDirective;
	}
	else
	{
		std::vector<String>	parameters;
		while (token.type == Token::PARAMETER)
		{
			parameters.push_back(token.lexeme);
			accept(Token::PARAMETER);
		}
		lexer.lookingFor = Token::NAME;
		Directive	directive(name, parameters, contextStack.top());
		expect(Token::SEMICOLON);
		return directive;
	}
}

Directive	Parser::parseBlock(const String& blockName)
{
	// TODO: this is assuming a block directive can't have parameters
	// which isn't true
	Directive	blockDirective(blockName, std::vector<String>(), contextStack.top());
	expect(Token::LCURLY);
	contextStack.push(blockDirective.name);
	while (token != Token::RCURLY)
	{
		Optional<Directive> directive = parseDirective();
		if (!directive.exists)
		{
			break ;
		}
		configValidator[directive.value.name].validate(directive.value);
	}
	expect(Token::RCURLY);
	contextStack.pop();
	return blockDirective;
}

/* TODO: handle multiple parameters
 * for now, we can only handle a single parameter because there's
 * currently no way to differentiate between a parameter and a key */
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
