/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 05:42:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/13 06:11:24 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include "String.hpp"
#include "Token.hpp"

struct	Lexer
{
	Lexer(const char* fileName);

	std::ifstream	configFile;
	String			input;
	size_t			charOffset;
	size_t			lineOffset;
	String			lexemeBuffer;

	Token			getNextToken();
	Token			gift(Token::TokenType);
	size_t			skipWhitespaces();
	void			tryIdentifier();
	
	


	static const Predicate	WSP;

};

#endif
