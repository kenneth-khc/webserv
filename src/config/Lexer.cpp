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

const Predicate Lexer::isWSP(" \t\n");

Lexer::Lexer(const char* fileName):
configFile(fileName),
input(configFile),
currentToken()
{

}

const Token&	Lexer::peek() const
{
	return currentToken;
}

Token&	Lexer::next()
{
	currentToken = getNextToken();
	std::cout << "Lex: " << currentToken.lexeme << '\n';
	return currentToken;
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
				if (std::isalpha(c.value))
				{
					// TODO: differentiate between an Identifier for a key
					// and a Word for a value
					tryIdentifier();
					tryWord();
					return token(Token::IDENTIFIER);
				}
				// TODO: an actual number token
				else if (std::isdigit(c.value))
				{
					tryNumber();
					return token(Token::IDENTIFIER);
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

void	Lexer::tryIdentifier()
{
	while (std::isalpha(*input) || std::isdigit(*input)
			|| *input == '_' || *input == '-')
	{
		lexemeBuffer += input.consume().value;
	}
}

void	Lexer::tryWord()
{
	while (std::isalpha(*input) || std::isdigit(*input)
			|| *input == '_' || *input == '-' || *input == '/'
			|| *input == '=' || *input == '[' || *input == ']')
	{
		lexemeBuffer += input.consume().value;
	}
}

void	Lexer::tryNumber()
{
	while (std::isdigit(*input))
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
	while (input.consume("\n").exists)
		++skipped;
	skipped += consumedComment.value_or("").size();
	lexemeBuffer.clear();
	return skipped;
}
