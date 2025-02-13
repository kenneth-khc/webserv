/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RDP.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/13 05:49:38 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RDP_HPP
#define RDP_HPP

#include <fstream>
#include <exception>
#include "String.hpp"
#include "Lexer.hpp"

struct	Parser
{
	Parser(const char* fileName);

	Lexer			lexer;
	std::ifstream	configFile;
	const String	config;
	size_t			lineOffset;
	size_t			charOffset;

	void				parseConfig();
	void				parseDirective();
	void				parseBlock();
	std::vector<String>	parseParameters();
	void				expect(bool);
	void				expect(Token::TokenType);

	class	UnexpectedToken : public std::exception {};

};

#endif
