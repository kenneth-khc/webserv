/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/19 22:51:42 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "Validator.hpp"
#include "String.hpp"
#include "ConfigErrors.hpp"
#include <dirent.h>
#include <vector>
#include <algorithm>

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

bool	validateParameterSize(const Directive& dir, size_t expected)
{
	if (dir.parameters.size() == expected)
	{
		return true;
	}
	else
	{
		throw InvalidParameterAmount();
	}
}

bool	validateParameterSize(const Directive& dir, size_t min, size_t max)
{
	if (dir.parameters.size() >= min &&
			dir.parameters.size() <= max)
	{
		return true;
	}
	else
	{
		throw InvalidParameterAmount();
	}
}

bool	validateEnclosingContext(const Directive& dir,
								 const std::vector<String>& allowedContexts)
{
	if (std::find(allowedContexts.begin(),
				  allowedContexts.end(),
				  dir.enclosingContext) != allowedContexts.end())
	{
		return true;
	}
	else
	{
		throw InvalidContext(dir.name, dir.enclosingContext);
	}
}

bool	validatePrefix(const Directive& dir)
{
	validateParameterSize(dir, 1);

	std::vector<String>	allowedContexts;
	allowedContexts.push_back("global");
	validateEnclosingContext(dir, allowedContexts);

	if (dir.parameters[0] == '/')
	{
		return true;
	}
	else
	{
		throw InvalidParameter(dir.parameters[0]);
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
	std::vector<String>	allowedContexts;
	allowedContexts.push_back("http");
	validateEnclosingContext(dir, allowedContexts);
	return true;
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
