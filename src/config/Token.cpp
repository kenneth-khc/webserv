/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 03:29:22 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:24:16 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"
#include "String.hpp"
#include <stdexcept>

Token::Token():
	type(),
	lexeme(),
	lineNum(0),
	columnNum(0)
{

}

const int Token::TOKEN_TYPES = 13;


Token::Token(const String& str):
	lexeme(str)
{

}

Token::Token(const String& str, size_t lineNum, size_t columnNum):
	lexeme(str),
	lineNum(lineNum),
	columnNum(columnNum),
	diagnostic(lineNum, columnNum)
{

}

Token::Token(const TokenType& type, const String& lexeme):
	type(type),
	lexeme(lexeme)
{

}

#include <iostream>
Token::Token(const TokenType& type, const String& lexeme,
			 size_t lineNum, size_t columnNum):
	type(type),
	lexeme(lexeme),
	lineNum(lineNum),
	columnNum(columnNum),
	diagnostic(lineNum, columnNum)
{
	std::cout << "Token |" << lexeme << "| at " << lineNum << ":" << columnNum << '\n';
}

Token::Token(const Token& other):
	type(other.type),
	lexeme(other.lexeme),
	lineNum(other.lineNum),
	columnNum(other.columnNum),
	diagnostic(other.diagnostic)
{

}

Token::operator TokenType() const
{
	return type;
}

const char*	Token::stringified[] = {
	"None", "Name", "Parameter", "Digit", "Alpha", "Whitespace", "Char",
	"DoubleQuote", "LeftCurly", "RightCurly", "Semicolon", "Hash", "Newline",
	"EndOfFile"
};

String	Token::stringify(Token::TokenType type)
{
	if (type >= TOKEN_TYPES)
	{
		String	msg = "Index " + String(type) +
					  " is out of bounds of TokenTypes of size " +
					  String(TOKEN_TYPES);
		throw std::out_of_range(msg.c_str());
	}
	return stringified[type];
}
