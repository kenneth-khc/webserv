/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 22:02:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/13 05:24:32 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "String.hpp"

struct	Token
{
	enum	TokenType
	{
		IDENTIFIER, DIGIT, ALPHA, WSP, CHAR, DQUOTE,
		LCURLY, RCURLY, SEMICOLON, HASH, NEWLINE, END_OF_FILE
	};

	TokenType	type;
	String		lexeme;

	Token(const String&);
	Token(const TokenType&, const String&);
	operator	TokenType() const;

};

#endif
