/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:03:46 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <exception>
#include "String.hpp"
#include "Lexer.hpp"
#include "Directive.hpp"

struct	Parser
{
	Parser(const char* fileName);

	Lexer				lexer;
	std::ifstream		configFile;
	const String		config;

	void				parseConfig();
	Directive			parseDirective();
	Directive			parseBlock();
	std::vector<String>	parseParameters();
	void				expect(bool);
	void				expect(Token::TokenType);

	class	UnexpectedToken : public std::exception {};
};

#endif
