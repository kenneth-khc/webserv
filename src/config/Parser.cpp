/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RDP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 22:28:04 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include "String.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"

Parser::Parser(const char *fileName):
lexer(fileName),
configFile(fileName),
config(configFile),
lineOffset(0),
charOffset(0)
{

}

void	Parser::parseConfig()
{
	try
	{
		while (lexer.next() != Token::END_OF_FILE)
		{
			parseDirective();
		}
	}
	catch (const UnexpectedToken& e)
	{
		std::cerr << "unexpected token you bozo\n";
	}
}

void	Parser::parseDirective()
{
	expect(Token::IDENTIFIER);
	Optional<String>	whitespaces = lexer.input.consumeUntilNot(lexer.isWSP);
	if (lexer.next() == Token::LCURLY)
	{
		parseBlock();
	}
	else
	{
		expect(whitespaces.exists);
		std::vector<String>	params = parseParameters();
		lexer.next();
		expect(Token::SEMICOLON);
	}
}

void	Parser::parseBlock()
{
	expect(Token::LCURLY);
	lexer.next();
	parseDirective();
	lexer.next();
	expect(Token::RCURLY);
}

/* TODO: handle multiple parameters
 * for now, we can only handle a single parameter because there's
 * currently no way to differentiate between a parameter and a key */
std::vector<String>	Parser::parseParameters()
{
	std::vector<String>	parameters;
	parameters.push_back(lexer.currentToken.lexeme);
	std::cout << "Param: " << lexer.currentToken.lexeme << '\n';
	return parameters;
}

void	Parser::expect(bool	yes)
{
	if (yes)
	{
		return ;
	}
	else
	{
		throw UnexpectedToken();
	}
}

void	Parser::expect(Token::TokenType type)
{
	if (lexer.peek().type == type)
	{
		return ;
	}
	else
	{
		std::cerr << "Expected " << Token::stringified[type] << " but got "
				  << Token::stringified[lexer.peek().type] << " instead...\n";
		throw UnexpectedToken();
	}
}
