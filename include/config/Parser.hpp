/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/05 21:46:06 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

/* A Recursive Descent Parser responsible for parsing a configuration file
 * that sets up the web server. */

#include <stack>
#include "Lexer.hpp"
#include "String.hpp"
#include "Directive.hpp"
#include "Validators.hpp"
#include "Configuration.hpp"
#include "Parameter.hpp"

/* The Parser gets hooked up to a Lexer that reads from a file, and it keeps
 * requesting Tokens from the Lexer until it is done reading the file */

class	Parser
{
public:
	Parser(const char* fileName);
	~Parser();

	Configuration*	parseConfig();

	Configuration*			config;
	Validators				validators;
	Lexer					lexer;
	Token					token;
	Token					accepted;
	std::stack<Directive*>	parents;

private:
	Parser();
	Parser(const Parser&);

	void					parseDirective();
	std::vector<Parameter>	parseParameters();
	void					parseSimple(const String&,
										const std::vector<Parameter>&,
										const Diagnostic&);
	void					parseBlock(const String&,
									   const std::vector<Parameter>&,
									   const Diagnostic&);
	void					expect(Token::TokenType);
	bool					accept(Token::TokenType);

	const String filename;
};

#endif
