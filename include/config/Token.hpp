/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 22:02:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:04:35 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "String.hpp"
#include "Diagnostic.hpp"


struct	Token
{
	static const int TOKEN_TYPES;
	enum	TokenType
	{
		NONE, NAME, PARAMETER, DIGIT, ALPHA, WSP, CHAR, DQUOTE,
		LCURLY, RCURLY, SEMICOLON, HASH, NEWLINE, END_OF_FILE
	};
	static const char* stringified[];
	static String		stringify(TokenType);

	TokenType	type;
	String		lexeme;

	size_t		lineNum;
	size_t		columnNum;
	Diagnostic	diagnostic;

	Token();
	Token(const String&);
	Token(const String&, size_t, size_t);
	Token(const TokenType&, const String&);
	Token(const TokenType&, const String&, size_t, size_t);

	Token(const Token&);
	operator	TokenType() const;
};

#endif
