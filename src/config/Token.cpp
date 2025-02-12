/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 03:29:22 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/13 05:24:31 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"
#include "String.hpp"

Token::Token(const String& str):
lexeme(str)
{

}

Token::Token(const TokenType& type, const String& lexeme):
type(type),
lexeme(lexeme)
{

}

Token::operator TokenType() const
{
	return type;
}
