/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 05:42:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/01 20:29:39 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

/* The Lexer reads from a file, tokenizing its contents and returning it to
 * the Parser to be parsed. */

#include <fstream>
#include "String.hpp"
#include "Token.hpp"

struct	Token;

class	Lexer
{
public:
	Lexer(const char* fileName);
	~Lexer();

	const Token&	peek() const;
	Token			advance();
	void			lookFor(Token::TokenType);
	
	static const Predicate	isWSP;

private:
	Lexer();
	Lexer(const Lexer&);

	std::ifstream		configFile;

	String				input;
	Token::TokenType	lookingFor;
	size_t				columnOffset;
	size_t				lineOffset;
	String				lexemeBuffer;
	Token				currentToken;

	Token			getNextToken();
	Token			tokenizeWord();
	void			tryName();
	void			tryParameter();
	void			skipWhitespaces();
	void			skipComment();
	Token			tokenize(Token::TokenType);
};

#endif
