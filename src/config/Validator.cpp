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

Validator::Validator(void (*function)(const Directive&)):
function(function)
{ }

void	Validator::operator()(const Directive& directive) const
{
	return function(directive);
}

void	no_op(const Directive&) { }

void	validateParameterSize(const Directive& dir, size_t expected)
{
	if (dir.parameters.size() != expected)
	{
		throw InvalidParameterAmount();
	}
}

void	validateParameterSize(const Directive& dir, size_t min, size_t max)
{
	if (dir.parameters.size() < min || dir.parameters.size() > max)
	{
		throw InvalidParameterAmount();
	}
}

void	validateEnclosingContext(const Directive& dir, Context context)
{
	if (dir.enclosingContext != context)
	{
		throw InvalidContext(dir.name, dir.enclosingContext);
	}
}

void	validateEnclosingContext(const Directive& dir,
								 const std::vector<Context>& allowedContexts)
{
	if (std::find(allowedContexts.begin(),
				  allowedContexts.end(),
				  dir.enclosingContext) == allowedContexts.end())
	{
		throw InvalidContext(dir.name, dir.enclosingContext);
	}
}

/*
Syntax : prefix absolutePath;
Default: —
Context: global */
void	validatePrefix(const Directive& dir)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, GLOBAL);

	const String&	path = dir.parameters[0];
	if (path[0] != '/')
	{
		throw InvalidParameter(path);
	}
}


// TODO: accept more options for listen. right now, the only parameter
// accepted is a single port number

/*
Syntax : listen port;
Default: —
Context: server */
void	validateListen(const Directive& dir)
{
	validateParameterSize(dir, 1);
	const String&	str = dir.parameters[0];
	int	portNum = str.toInt();

	if (portNum < 0 || portNum > 65535)
	{
		throw InvalidParameter(str);
	}
}

/*
Syntax : http { ... }
Default: —
Context: main */
void	validateHTTP(const Directive& dir)
{
	validateParameterSize(dir, 0);
	validateEnclosingContext(dir, GLOBAL);
}

void	validateServer(const Directive& dir)
{
	validateEnclosingContext(dir, HTTP);
}

/*
Syntax : location uri { ... }
Default: —
Context: server, location */
void	validateLocation(const Directive& dir)
{
	// TODO: support exact matches?
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, (SERVER, LOCATION));
}

/*
Syntax : root path;
Default: root html;
Context: http, server, location, if in location */
void	validateRoot(const Directive& dir)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, (HTTP, SERVER, LOCATION));
}

/*
Syntax : index file ...;
Default: index index.html;
Context: http, server, location */
void	validateIndex(const Directive& dir)
{
	validateEnclosingContext(dir, (HTTP, SERVER, LOCATION));
}
