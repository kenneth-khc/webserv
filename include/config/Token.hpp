/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 22:02:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/04 16:29:45 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "String.hpp"
#include "Diagnostic.hpp"

struct	Token
{
	enum	TokenType
	{
		NONE,
		NAME,
		PARAMETER,
		DQUOTE,
		LCURLY,
		RCURLY,
		SEMICOLON,
		HASH,
		NEWLINE,
		END_OF_FILE
	};
	/** number of types of tokens */
	static const int 	TOKEN_TYPES;

	/** array holding string representation of the types */
	static const char*	STRINGIFIED[];

	/** Construct a default Token with nothing set */
	Token();

	/** Construct a Token with a given Type and lexeme */
	Token(const TokenType&, const String&);

	/** Construct a complete Token with a given Type, lexeme and diagnostic information */
	Token(const TokenType&, const String&, const Diagnostic&);

	/** Copy from another Token */
	Token(const Token&);

	/** Stringify a Token's Type */
	static String	stringify(TokenType);

	/** Implicitly convert a Token into its TokenType */
	operator	TokenType() const;

	/** type of this token */
	TokenType	type;

	/** lexeme of this token */
	String		lexeme;

	/** diagnostic information for this token */
	Diagnostic	diagnostic;
};

#endif
