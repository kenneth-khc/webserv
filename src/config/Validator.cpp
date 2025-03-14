/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/01 18:38:30 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "Validator.hpp"
#include "String.hpp"
#include "ConfigErrors.hpp"
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <limits>

Validator::Validator(void (*function)(const Directive&,
									  const Mappings&)):
function(function)
{ }

void	Validator::operator()(const Directive& directive,
							  const Mappings& mappings) const
{
	return function(directive, mappings);
}

void	no_op(const Directive&, const Mappings&) { }

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

// TODO: right now, directives either allow duplicates or they don't.
//		 is there a situation where I need to allow a frequency of a certain
//		 number rather than just 0 or 1?
void	validateDuplicateDirective(const Directive& dir,
								   const Mappings& mappings)
{
	DirectiveRange	range = mappings.equal_range(dir.name);
	if (range.first == mappings.end())
	{
		return ;
	}
	size_t	count = std::distance(range.first, range.second);
	if (count > 0)
	{
		throw DuplicateDirective(dir);
	}
}

/*
Syntax : prefix absolutePath;
Default: —
Context: global
Count  : 1 */
void	validatePrefix(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, GLOBAL);
	validateDuplicateDirective(dir, mappings);

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
void	validateListen(const Directive& dir, const Mappings&)
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
Context: main
Count  : 1 */
void	validateHTTP(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 0);
	validateEnclosingContext(dir, GLOBAL);
	validateDuplicateDirective(dir, mappings);
}

/*
Syntax : server { ... }
Default: —
Context: http */
void	validateServer(const Directive& dir, const Mappings&)
{
	validateEnclosingContext(dir, HTTP);
}

/*
Syntax : server_name name;
Default : —
Context: server
*/
void	validateServerName(const Directive& dir, const Mappings&)
{
	validateParameterSize(dir, 1, std::numeric_limits<size_t>::max());
	validateEnclosingContext(dir, SERVER);
}

/*
Syntax : location uri { ... }
Default: —
Context: server, location */
void	validateLocation(const Directive& dir, const Mappings&)
{
	// TODO: support exact matches?
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, (SERVER, LOCATION));
}

/*
Syntax : root path;
Default: root html;
Context: http, server, location, if in location */
void	validateRoot(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, (HTTP, SERVER, LOCATION));
	validateDuplicateDirective(dir, mappings);
}

/*
Syntax : index file ...;
Default: index index.html;
Context: http, server, location */
void	validateIndex(const Directive& dir, const Mappings&)
{
	validateEnclosingContext(dir, (HTTP, SERVER, LOCATION));
}

/*
Syntax : types file;
Default: —
Context: http */
void	validateTypes(const Directive&, const Mappings&)
{

}
