/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 03:00:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:43:44 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR
#define VALIDATOR

#include <exception>
#include "Directive.hpp"
#include "String.hpp"

/* A Validator is a function object that validates directives.
 * The idea is that the rules and contraints of a directive gets stored here,
 * and each supported directive is associated with its own Validator.
 * When a directive is parsed, we find it in our list of supported directives
 * and call its Validator to ensure the parameters are valid */

struct	Validator
{

	Validator();
	Validator(bool(*)(const Directive&));

	bool	(*validate)(const Directive& parameter);
	bool	operator()(Directive parameter) const;
};

struct	InvalidDirective : public std::exception
{
	InvalidDirective(const String& directive):
		directive(directive) {}

	~InvalidDirective() throw() {}

	String	directive;
};

struct	InvalidContext : public std::exception
{
	InvalidContext(const String& directive, const String& context):
		directive(directive),
		context(context) {}

	~InvalidContext() throw() {}

	String	directive;
	String	context;
};

bool	returnsTrue(const Directive&);
bool	checkIfAbsolutePath(const Directive&);
bool	isValidPortNumber(const Directive&);
bool	validateHTTP(const Directive& );

#endif
