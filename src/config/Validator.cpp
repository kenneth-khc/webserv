/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/24 16:25:39 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "Validator.hpp"
#include "String.hpp"
#include "ConfigErrors.hpp"
#include "VectorInitializer.hpp"
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

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

void	validateParameterValues(const std::vector<String>& got,
								const std::vector<String>& expected)
{
	for (size_t i = 0; i < got.size(); ++i)
	{
		if (std::find(expected.begin(),
					  expected.end(),
					  got[i]) == expected.end())
		{
			throw InvalidParameter(got[i]);
		}
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
	Directive::EqualRange	range = mappings.equal_range(dir.name);
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
Syntax : prefix absolute_path;
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
	validateEnclosingContext(dir, vector_of(SERVER)(LOCATION));
}

/*
Syntax : root path;
Default: root html;
Context: http, server, location, if in location */
void	validateRoot(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, vector_of(HTTP)(SERVER)(LOCATION));
	validateDuplicateDirective(dir, mappings);
}

/*
Syntax : index file ...;
Default: index index.html;
Context: http, server, location */
void	validateIndex(const Directive& dir, const Mappings& mappings)
{
	validateEnclosingContext(dir, vector_of(HTTP)(SERVER)(LOCATION));
	validateParameterSize(dir, 1, std::numeric_limits<size_t>::max());
	validateDuplicateDirective(dir, mappings);
}

/*
Syntax : types filename;
Default: —
Context: http, server, location */
void	validateTypes(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, HTTP);
	validateDuplicateDirective(dir, mappings);
}

void	validateBoolean(const String& str)
{
	if (str != "on" && str != "off")
	{
		throw InvalidParameter(str);
	}
}

/*
Syntax : autoindex boolean;
Default: off
Context: http, server, location */
void	validateAutoindex(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, vector_of(HTTP)(SERVER)(LOCATION));
	validateDuplicateDirective(dir, mappings);
	validateBoolean(dir.parameters[0]);
}

/*
Syntax : ExecCGI boolean;
Default: off
Context: location */
void	validateExecCGI(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, LOCATION);
	validateDuplicateDirective(dir, mappings);
	validateBoolean(dir.parameters[0]);
}

void	validateFileExtension(const String& str)
{
	if (str[0] != '.')
	{
		throw InvalidParameter(str);
	}
}

/*
Syntax : CGI-script file-extension ... ;
Default: —
Context: location */
void	validateCGIScript(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1, std::numeric_limits<size_t>::max());
	validateEnclosingContext(dir, LOCATION);
	validateDuplicateDirective(dir, mappings);
	const std::vector<String>&	parameters = dir.parameters;
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		validateFileExtension(parameters[i]);
	}
}

/*
Syntax : allow_method methods ... ;
Default: —
Context: location */
void	validateAllowMethod(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1, 3);
	validateParameterValues(dir.parameters,
							vector_of<String>("GET")("POST")("DELETE")("HEAD"));
	validateEnclosingContext(dir, LOCATION);
	validateDuplicateDirective(dir, mappings);
}

/*
Syntax : client_max_body_size size;
Default: 1m
Context: http, server, location */
void	validateClientMaxBodySize(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, vector_of(HTTP)(SERVER)(LOCATION));
	validateDuplicateDirective(dir, mappings);

	// validate size format
	const String&	size = dir.parameters[0];
	for (size_t i = 0; i < size.length() - 1; ++i)
	{
		if (!std::isdigit(size[i]))
		{
			throw InvalidParameter(size);
		}
	}
	if (!(std::isdigit(size.back()) ||
		  size.back() == 'k' || size.back() == 'K' ||
		  size.back() == 'm' || size.back() == 'M'))
	{
		throw InvalidParameter(size);
	}
}

void	validateErrorCode(const String& code)
{
	static std::vector<String>	errorCodes = vector_of<String>
	("300")("301")("302")("303")("304")("305")("306")("307")("308")
	("400")("401")("402")("403")("404")("405")("406")("407")("408")("409")("410")
	("411")("412")("413")("414")("415")("416")("417")("418")("421")("422")("423")
	("424")("425")("426")("428")("429")("431")("451")
	("500")("501")("502")("503")("504")("505")("506")("507")("508")("510")("511");

	if (std::find(errorCodes.begin(),
				  errorCodes.end(), code) == errorCodes.end())
	{
		throw InvalidParameter(code);
	}
}

void	validateAcceptUploads(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, LOCATION);
	validateDuplicateDirective(dir, mappings);
	validateBoolean(dir.parameters[0]);
}

/*
Syntax : upload_directory file_path;
Default: —
Context: location */
void	validateUploadDirectory(const Directive& dir, const Mappings& mappings)
{
	validateParameterSize(dir, 1);
	validateEnclosingContext(dir, LOCATION);
	validateDuplicateDirective(dir, mappings);
}

/*
Syntax : error_page code ... uri;
Default: —
Context: server */
void	validateErrorPage(const Directive& dir, const Mappings& mappings)
{
	validateEnclosingContext(dir, vector_of(HTTP)(SERVER)(LOCATION));
	validateDuplicateDirective(dir, mappings);

	const std::vector<String>&	parameters = dir.parameters;
	for (size_t i = 0; i < parameters.size()-1; ++i)
	{
		validateErrorCode(parameters[i]);
	}
	// TODO: validate file format
}

