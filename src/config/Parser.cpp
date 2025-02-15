/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:42:40 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <stdexcept>
#include "Parameter.hpp"
#include "String.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "ConfigValidator.hpp"
#include "Validator.hpp"

Parser::Parser(const char *fileName):
lexer(fileName),
configFile(fileName),
config(configFile)
{

}

void	ConfigValidator::add(const String& name, const Validator& validator)
{
	directives.insert(std::make_pair(name, validator));

}

const Validator&	ConfigValidator::operator[](const String& key) const
{
	try
	{
		const Validator&	validator = directives.at(key);
		return validator;
	}
	catch (const std::out_of_range& e)
	{
		throw InvalidDirective(key);
	}
}

void	Parser::parseConfig()
{
	contextStack.push("global");

	try
	{
		while (lexer.next() != Token::END_OF_FILE)
		{
			Directive	directive = parseDirective();
			configValidator[directive.name].validate(directive);
		}
	}
	catch (const UnexpectedToken& e)
	{
		std::cerr << "unexpected token you bozo\n";
	}
	catch (const InvalidParameter& e)
	{
		std::cerr << e.parameter << " is an invalid parameter you bozo\n";
	}
	catch (const InvalidDirective& e)
	{
		std::cerr << e.directive << " is an invalid directive you bozo\n";
	}
	catch (const InvalidContext& e)
	{
		std::cerr << e.directive << " is in an invalid context " << e.context << '\n';
	}
}

Directive	Parser::parseDirective()
{
	expect(Token::IDENTIFIER);
	String	name = lexer.peek().lexeme;
	Optional<String>	whitespaces = lexer.input.consumeUntilNot(lexer.isWSP);
	if (lexer.next() == Token::LCURLY)
	{
		// TODO: parsing a block is hella sus. fix this shit
		Directive	blockDirective(name, "", contextStack.top());
		configValidator[blockDirective.name].validate(blockDirective);
		return parseBlock();
	}
	else
	{
		expect(whitespaces.exists);
		std::vector<String>	params = parseParameters();
		Directive	directive(name, params[0], contextStack.top());
		// validate directive

		lexer.next();
		expect(Token::SEMICOLON);
		return directive;
	}
}

Directive	Parser::parseBlock()
{
	expect(Token::LCURLY);
	lexer.next();
	Directive directive = parseDirective();
	lexer.next();
	expect(Token::RCURLY);
	return directive;
}

/* TODO: handle multiple parameters
 * for now, we can only handle a single parameter because there's
 * currently no way to differentiate between a parameter and a key */
std::vector<String>	Parser::parseParameters()
{
	std::vector<String>	parameters;
	parameters.push_back(lexer.currentToken.lexeme);
	std::cout << "Param: " << lexer.currentToken.lexeme << '\n';
	return parameters;
}

void	Parser::expect(bool	yes)
{
	if (yes)
	{
		return ;
	}
	else
	{
		throw UnexpectedToken();
	}
}

void	Parser::expect(Token::TokenType type)
{
	if (lexer.peek().type == type)
	{
		return ;
	}
	else
	{
		std::cerr << "Expected " << Token::stringified[type] << " but got "
				  << Token::stringified[lexer.peek().type] << " instead...\n";
		throw UnexpectedToken();
	}
}
