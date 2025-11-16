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
#include "Diagnostic.hpp"
#include "String.hpp"
#include <stdexcept>

const int Token::TOKEN_TYPES = 10;

const char*	Token::STRINGIFIED[TOKEN_TYPES] = {
	"None",
	"Name",
	"Parameter",
	"DoubleQuote",
	"LeftCurly",
	"RightCurly",
	"Semicolon",
	"Hash",
	"Newline",
	"EndOfFile",
};


Token::Token():
	type(NONE),
	lexeme(""),
	diagnostic()
{
}

Token::Token(const TokenType& type, const String& lexeme):
	type(type),
	lexeme(lexeme),
/** diagnostics are ignored and irrelevant in this case */
	diagnostic()
{
}

Token::Token(const TokenType& type,
			 const String& lexeme,
			 const Diagnostic& diagnostic):
	type(type),
	lexeme(lexeme),
	diagnostic(diagnostic)
{
}

Token::Token(const Token& other):
	type(other.type),
	lexeme(other.lexeme),
	diagnostic(other.diagnostic)
{
}

Token::operator TokenType() const
{
	return type;
}

String	Token::stringify(Token::TokenType type)
{
	if (type >= TOKEN_TYPES)
	{
		String	msg = "Index " + String(type) +
					  " is out of bounds of TokenTypes of size " +
					  String(TOKEN_TYPES);
		throw std::out_of_range(msg.c_str());
	}
	return STRINGIFIED[type];
}
