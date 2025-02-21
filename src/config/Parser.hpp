/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/21 18:47:43 by kecheong         ###   ########.fr       */
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
#include "Configuration.hpp"

struct	Parser
{
	Parser(const char* fileName);

	Lexer				lexer;
	std::ifstream		configFile;
	ConfigValidator		configValidator;
	Token				token;
	std::stack<Context>	contexts;


	Configuration		parseConfig();
	Optional<Directive>	parseDirective();
	Directive			parseBlock(Context);
	std::vector<String>	parseParameters();
	void				expect(Token::TokenType);
	bool				accept(Token::TokenType);
};

#endif
