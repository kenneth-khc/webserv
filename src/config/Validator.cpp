/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:43:10 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "Validator.hpp"
#include "Parameter.hpp"
#include "String.hpp"

Validator::Validator()
{ }

Validator::Validator(bool (*function)(const Directive&)):
validate(function)
{ }

bool	Validator::operator()(Directive parameter) const
{
	return validate(parameter);
}

bool	returnsTrue(const Directive&)
{
	return true;
}

bool	checkIfAbsolutePath(const Directive& dir)
{
	const String&	str = dir.param;
	if (str[0] == '/')
	{
		return true;
	}
	else
	{
		throw InvalidParameter(str);
	}
}

bool	isValidPortNumber(const Directive& dir)
{
	const String&	str = dir.param;
	int	portNum = str.toInt();

	if (portNum > 0 && portNum < 65536)
	{
		return true;
	}
	else
	{
		throw InvalidParameter(str);
	}
}

bool	validateHTTP(const Directive& dir)
{
	const String&	context = dir.enclosingContext;
	const String&	name = dir.name;
	if (dir.enclosingContext == String("global"))
	{
		return true;
	}
	else
	{
		throw InvalidContext(name, context);
	}
}
