/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/21 18:45:24 by kecheong         ###   ########.fr       */
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
								 const std::vector<Context>& allowedContexts)
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

	std::vector<Context>	allowedContexts;
	allowedContexts.push_back(GLOBAL);
	validateEnclosingContext(dir, allowedContexts);

	const String&	path = dir.parameters[0];
	if (path[0] == '/')
	{
		return true;
	}
	else
	{
		throw InvalidParameter(path);
	}
}

// TODO: accept more options for validate. right now, the only parameter
// accepted is a single port number
bool	validateListen(const Directive& dir)
{
	validateParameterSize(dir, 1);
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
	validateParameterSize(dir, 0);
	const String&	context = dir.enclosingContext;
	const String&	name = dir.name;
	if (context == GLOBAL)
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
	std::vector<Context>	allowedContexts;
	allowedContexts.push_back(HTTP);
	validateEnclosingContext(dir, allowedContexts);
	return true;
}

bool	validateLocation(const Directive& dir)
{
	// TODO: support exact matches?
	validateParameterSize(dir, 1);

	return true;
}

bool	validateRoot(const Directive& dir)
{
	validateParameterSize(dir, 1);
	
	std::vector<Context>	allowedContexts;
	allowedContexts.push_back(HTTP);
	allowedContexts.push_back(SERVER);
	allowedContexts.push_back(LOCATION);
	validateEnclosingContext(dir, allowedContexts);
	return true;
}

bool	validateIndex(const Directive&)
{
	return true;
}
