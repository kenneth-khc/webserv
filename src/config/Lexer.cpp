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
#include <stdexcept>

const Predicate Lexer::isWSP(" \t\n");

Lexer::Lexer(const char* fileName):
	configFile(fileName),
	input(configFile),
	lookingFor(Token::NAME),
	columnOffset(1),
	lineOffset(1),
	lexemeBuffer(),
	currentToken()
{
	if (!configFile.is_open())
	{
		std::cerr << "Error. Cannot open file <" << fileName << ">\n";
		std::exit(EXIT_FAILURE);
	}
}

Lexer::~Lexer()
{

}

void	Lexer::lookFor(Token::TokenType type)
{
	if (type != Token::NAME && type != Token::PARAMETER)
	{
		String	msg = "Invalid TokenType " + Token::stringify(type) + " to look for";
		throw std::invalid_argument(msg.c_str());
	}
	lookingFor = type;
}

Token	Lexer::tokenizeWord()
{
	if (lookingFor == Token::NAME)
	{
		tryName();
	}
	else if (lookingFor == Token::PARAMETER)
	{
		tryParameter();
	}
	return tokenize(lookingFor);
}

Token	Lexer::getNextToken()
{
	skipWhitespaces();

	Optional<char>	c;
	while ((c = input.consume()).exists)
	{
		lexemeBuffer += c.value;
		switch (c.value)
		{
			default:
				return tokenizeWord();
			case '{':
				return tokenize(Token::LCURLY);
			case '}':
				return tokenize(Token::RCURLY);
			case ';':
				return tokenize(Token::SEMICOLON);
			case '\n':
				return tokenize(Token::NEWLINE);
			case '#':
				skipComment();
		}
	}
	return Token(Token::END_OF_FILE, "EOF");
}

const Token&	Lexer::peek() const
{
	return currentToken;
}

Token	Lexer::advance()
{
	currentToken = getNextToken();
	columnOffset += currentToken.lexeme.length();
	/*std::cout << Token::stringified[currentToken.type] << ": "*/
	/*		  << currentToken.lexeme << '\n';*/
	return currentToken;
}

void	Lexer::tryName()
{
	for (char c = input.front();
		 std::isalpha(c) || std::isdigit(c) || c == '_' || c == '-';
		 c = input.front())
	{
		lexemeBuffer += input.consume().value;
	}
}

void	Lexer::tryParameter()
{
	for (char c = input.front();
		!isWSP(c) && c != ';' && c != '{' && c != '}';
		c = input.front())
	{
		lexemeBuffer += input.consume().value;
	}
}

Token	Lexer::tokenize(Token::TokenType type)
{
	String	lexeme = lexemeBuffer;
	lexemeBuffer.clear();
	return Token(type, lexeme, lineOffset, columnOffset);
}

void	Lexer::skipWhitespaces()
{
	for (char c = input.front();
		 c == ' ' || c == '\t' || c == '\n';
		 c = input.front())
	{
		if (c == '\n')
		{
			++lineOffset;
			columnOffset = 1;
		}
		else
		{
			++columnOffset;
		}
		input.consume();
	}
}

void	Lexer::skipComment()
{
	Optional<String>	consumedComment = input.consumeUntil("\n");
	skipWhitespaces();
	lexemeBuffer.clear();
}
