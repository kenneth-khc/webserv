/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 18:40:30 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <stack>
#include "Lexer.hpp"
#include "String.hpp"
#include "Directive.hpp"
#include "Configurator.hpp"
#include "Configuration.hpp"

struct	Parser
{
	Parser(const char* fileName);

	Configurator		configurator;
	Lexer				lexer;
	Token				token;
	std::stack<Context>	contexts;
	std::stack< std::multimap<String,Directive> >	mapStack;

	Configuration		parseConfig();
	Directive			parseDirective();
	std::vector<String>	parseParameters();
	Directive			parseSimple(const String&,
									const std::vector<String>&);
	Directive			parseBlock(const String&,
								   const std::vector<String>&);

	void				expect(Token::TokenType);
	bool				accept(Token::TokenType);
};

#endif
