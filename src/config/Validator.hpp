/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 03:00:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:13:21 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR
#define VALIDATOR

#include <exception>
#include "String.hpp"

/* A Validator is a function object that validates directives.
 * The idea is that the rules and contraints of a directive gets stored here,
 * and each supported directive is associated with its own Validator.
 * When a directive is parsed, we find it in our list of supported directives
 * and call its Validator to ensure the parameters are valid */

struct	Validator
{

	Validator();
	Validator(bool(*)(const String&));

	bool	(*validate)(const String& parameter);
	bool	operator()(String parameter) const;
};

struct InvalidDirective : public std::exception
{
	InvalidDirective(const String& directive):
		directive(directive) {}

	~InvalidDirective() throw() {}

	String	directive;
};

bool	checkIfAbsolutePath(const String&);

#endif
