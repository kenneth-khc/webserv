/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 05:44:48 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/13 06:15:44 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include "Token.hpp"
#include <cctype>

const Predicate Lexer::WSP(" \t\n");

Lexer::Lexer(const char* fileName):
configFile(fileName),
input(configFile)
{

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
				return gift(Token::LCURLY);
			case '}':
				return gift(Token::RCURLY);
			case ';':
				return gift(Token::SEMICOLON);
			case '#':
				return gift(Token::HASH);
			case '\n':
				return gift(Token::NEWLINE);
			default:
				if (std::isalpha(c.value))
				{
					tryIdentifier();
					return gift(Token::IDENTIFIER);
				}

		}
	}
	return gift(Token::END_OF_FILE);
}

Token	Lexer::gift(Token::TokenType type)
{
	String	lexeme = lexemeBuffer;
	lexemeBuffer.clear();
	return Token(type, lexeme);
}

void	Lexer::tryIdentifier()
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

	while (input.consumeIf(WSP).exists)
	{
		++skipped;
	}
	return skipped;
}
