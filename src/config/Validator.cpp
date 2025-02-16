/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 17:12:20 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "Validator.hpp"
#include "String.hpp"
#include "ConfigErrors.hpp"
#include <dirent.h>

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

bool	validatePrefix(const Directive& dir)
{
	if (dir.parameters.size() != 1)
	{
		throw InvalidParameterAmount();
	}
	if (dir.enclosingContext != "global")
	{
		throw InvalidContext(dir.name, dir.enclosingContext);
	}
	const String&	str = dir.parameters[0];
	if (str == "")
	{
		std::cerr << "Missing parameter in " << dir.name << '\n';
		throw InvalidParameter();
	}
	if (str[0] == '/')
	{
		return true;
	}
	else
	{
		throw InvalidParameter(str);
	}
}

// TODO: accept more options for validate. right now, the only parameter
// accepted is a single port number
bool	validateListen(const Directive& dir)
{
	const String&	str = dir.parameters[0];
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
	if (dir.enclosingContext == "global")
	{
		return true;
	}
	else
	{
		throw InvalidContext(name, context);
	}
}

bool	validateServer(const Directive& dir)
{
	const String& context = dir.enclosingContext;
	const String& name = dir.name;
	if (dir.enclosingContext == "http")
	{
		return true;
	}
	else
	{
		throw InvalidContext(name, context);
	}
}

bool	validateLocation(const Directive&)
{
	return true;
}

bool	validateRoot(const Directive&)
{
	return true;
}

bool	validateIndex(const Directive&)
{
	return true;
}
