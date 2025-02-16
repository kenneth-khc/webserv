/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 17:13:13 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <stack>
#include "Lexer.hpp"
#include "String.hpp"
#include "Directive.hpp"
#include "ConfigValidator.hpp"
#include "Optional.hpp"

struct	Parser
{
	Parser(const char* fileName);

	Lexer				lexer;
	std::ifstream		configFile;
	ConfigValidator		configValidator;
	std::stack<String>	contextStack;
	Token				token;

	void				parseConfig();
	Optional<Directive>	parseDirective();
	Directive			parseBlock(const String&);
	std::vector<String>	parseParameters();
	void				expect(Token::TokenType);
	bool				accept(Token::TokenType);
};

#endif
