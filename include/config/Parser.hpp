/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:12:47 by kecheong         ###   ########.fr       */
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
#include "Configurator.hpp"
#include "Configuration.hpp"
#include "Parameter.hpp"

/* The Parser gets hooked up to a Lexer that reads from a file, and it keeps
 * requesting Tokens from the Lexer until it is done reading the file */

class	Parser
{
public:
	Parser(const char* fileName);
	~Parser();

	Configurator		configurator;
	Lexer				lexer;
	Token				token;
	std::stack<Context>	contexts;
	std::stack< std::multimap<String,Directive*> >	mapStack;

	// TODO: wtf am i doing
	std::stack<Directive*>	parents;

	Token				accepted;

	Configuration			parseConfig();
	Directive*				parseDirective();
	std::vector<Parameter>	parseParameters();
	Directive*				parseSimple(const String&,
										const std::vector<Parameter>&);
	Directive*				parseBlock(const String&,
										const std::vector<Parameter>&);

	void					expect(Token::TokenType);
	bool					accept(Token::TokenType);

private:
	Parser();
	Parser(const Parser&);
};

#endif
