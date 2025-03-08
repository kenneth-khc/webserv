/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 05:44:48 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/08 19:52:10 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include "Token.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>

const Predicate Lexer::isWSP(" \t\n");

Lexer::Lexer(const char* fileName):
configFile(fileName),
input(configFile),
currentToken(),
lookingFor(Token::NAME)
{
	if (!configFile.is_open())
	{
		std::cerr << "Error. Cannot open file <" << fileName << ">\n";
		std::exit(EXIT_FAILURE);
	}
}

const Token&	Lexer::peek() const
{
	return currentToken;
}

Token&	Lexer::advance()
{
	currentToken = getNextToken();
	/*std::cout << Token::stringified[currentToken.type] << ": "*/
	/*		  << currentToken.lexeme << '\n';*/
	return currentToken;
}

void	Lexer::tryParameter()
{
	while (!isWSP(*input) && *input != ';' && *input != '{' && *input != '}')
	{
		lexemeBuffer += input.consume().value;
	}
}

Token	Lexer::getNextToken()
{
	charOffset += skipWhitespaces();

	Optional<char>	c;
	while ((c = input.consume()).exists)
	{
		lexemeBuffer += c.value;
		switch (c.value)
		{
			case '{':
				return token(Token::LCURLY);
			case '}':
				return token(Token::RCURLY);
			case ';':
				return token(Token::SEMICOLON);
			case '#':
				skipComment(); break; // TODO: make this better
			case '\n':
				return token(Token::NEWLINE);
			default:
				if (lookingFor == Token::NAME)
				{
					tryName();
					return token(Token::NAME);
				}
				else if (lookingFor == Token::PARAMETER)
				{
					tryParameter();
					return token(Token::PARAMETER);
				}
		}
	}
	return Token(Token::END_OF_FILE, "EOF");
}

Token	Lexer::token(Token::TokenType type)
{
	String	lexeme = lexemeBuffer;
	lexemeBuffer.clear();
	return Token(type, lexeme);
}

void	Lexer::tryName()
{
	while (std::isalpha(*input) || std::isdigit(*input)
			|| *input == '_' || *input == '-')
	{
		lexemeBuffer += input.consume().value;
	}
}

size_t	Lexer::skipWhitespaces()
{
	size_t	skipped = 0;
	while (input.consumeIf(isWSP).exists)
	{
		++skipped;
	}
	return skipped;
}

size_t	Lexer::skipComment()
{
	size_t	skipped = 0;
	Optional<String>	consumedComment = input.consumeUntil("\n");
	skipped += skipWhitespaces();
	/*while (input.consume("\n").exists)*/
	/*	++skipped;*/
	skipped += consumedComment.value_or("").size();
	lexemeBuffer.clear();
	return skipped;
}
