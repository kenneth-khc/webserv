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

#include "String.hpp"
#include "Token.hpp"

struct	Token;

class	Lexer
{
public:
	/** Construct a Lexer that is ready to tokenize the given filename */
	Lexer(const char* fileName);
	~Lexer();

	/** Returns a reference to the current token */
	const Token&	peek() const;

	/** Advances the Lexer to grab the next Token and returns it */
	Token			advance();

	/** Toggle between the states of looking for a Name and a Parameter */
	void			lookFor(Token::TokenType);
	
	static const Predicate	isWSP;

private:
	/** Not expected to default construct a Lexer */
	Lexer();
	/** Not expected to copy construct a Lexer */
	Lexer(const Lexer&);

	Token	getNextToken();
	Token	tokenizeWord();
	void	tryName();
	void	tryParameter();
	void	skipWhitespaces();
	void	skipComment();
	Token	tokenize(Token::TokenType);

	/** Name of the file to be tokenized */
	const String	filename;

	/** The contents of the config file streamed into a single String */
	String			input;

	/** Current token found */
	Token			currentToken;

	/** The current Type to be prioritizing, toggling between Name and Parameter */
	Token::TokenType	lookingFor;

	/** Buffer holding the lexeme for the current token */
	String			lexemeBuffer;

	/** Current line offset of the scanner, starts at 1 on each file */
	size_t			lineOffset;

	/** Current column offset of the scanner, starts at 1 on each line */
	size_t			columnOffset;
};

#endif
