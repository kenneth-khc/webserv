/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigErrors.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:10:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 05:17:21 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* These are custom exceptions thrown by the Parser, to indicate things
 * going wrong when parsing a config file. It provides the ability to display
 * the error to the user, allowing them to correct the config file. */

#include <exception>
#include "String.hpp"

class	ConfigError : public std::exception
{

};

class	UnexpectedToken : public ConfigError
{

};

class	InvalidParameter : public ConfigError
{
public:
	String	parameter;

	InvalidParameter(const String& str): parameter(str) { }
	~InvalidParameter() throw() { }
};

class	InvalidDirective : public ConfigError
{
public:
	String	directive;

	InvalidDirective(const String& directive): directive(directive) { }
	~InvalidDirective() throw() { }
};

class	InvalidContext : public ConfigError
{
public:
	String	directive;
	String	context;

	InvalidContext(const String& directive, const String& context):
		directive(directive),
		context(context) { }

	~InvalidContext() throw() { }
};
