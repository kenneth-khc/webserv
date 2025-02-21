/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigErrors.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:10:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/21 18:45:16 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGERRORS_HPP
#define CONFIGERRORS_HPP

/* These are custom exceptions thrown by the Parser, to indicate things
 * going wrong when parsing a config file. It provides the ability to display
 * the error to the user, allowing them to correct the config file if they
 * so desire. */

#include <exception>
#include "String.hpp"
#include "Token.hpp"

class	ConfigError : public std::exception
{
public:
	~ConfigError() throw();
	const char*	what() const throw();

protected:
	String	msg;
};

class	UnexpectedToken : public ConfigError
{
public:
	UnexpectedToken(Token::TokenType expected, Token::TokenType got);
	~UnexpectedToken() throw();
};

class	InvalidParameterAmount : public ConfigError
{
public:
	InvalidParameterAmount();
	~InvalidParameterAmount() throw();
};

class	InvalidParameter : public ConfigError
{
public:
	InvalidParameter();
	InvalidParameter(const String& str);
	~InvalidParameter() throw();
};

class	InvalidDirective : public ConfigError
{
public:
	InvalidDirective(const String& directive);
	~InvalidDirective() throw();
};

class	InvalidContext : public ConfigError
{
public:
	InvalidContext(const String& directive, const String& context);
	~InvalidContext() throw();
};

#endif
