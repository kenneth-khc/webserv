/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:08:51 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Context.hpp"
#include "Directive.hpp"
#include "Validator.hpp"
#include "String.hpp"
#include "InvalidParameter.hpp"
#include "InvalidParameterAmount.hpp"
#include "InvalidDirective.hpp"
#include "InvalidContext.hpp"
#include "DuplicateDirective.hpp"
#include "VectorInitializer.hpp"
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

Validator::Validator(ValidationFunc function):
function(function)
{
}

void	Validator::operator()(const Directive& directive,
							  const Directive::Map& mappings) const
{
	return function(directive, mappings);
}

void	no_op(const Directive&, const Directive::Map&)
{
}

static size_t	countFrequency(const String& name, const Directive::Map& mappings)
{
	Directive::EqualRange	range = mappings.equal_range(name);
	size_t					count = std::distance(range.first, range.second);
	return count;
}

void	validateParameterSize(const Directive& directive, size_t expected)
{
	if (directive.parameters.size() != expected)
	{
		throw InvalidParameterAmount(directive, expected);
	}
}

void	validateParameterSize(const Directive& directive, size_t min, size_t max)
{
	if (directive.parameters.size() < min || directive.parameters.size() > max)
	{
		throw InvalidParameterAmount(directive, min, max);
	}
}

void	validateParameterValues(const Directive& directive,
								const std::vector<Parameter>& expected)
{
	const std::vector<Parameter>&	got = directive.parameters;
	for (size_t i = 0; i < got.size(); ++i)
	{
		if (std::find(expected.begin(),
					  expected.end(),
					  got[i]) == expected.end())
		{
			throw InvalidParameter(directive, got[i]);
		}
	}
}

void	validateEnclosingContext(const Directive& directive, Context::Context expected)
{
	Context::Context	context;
	try
	{
		context = directive.getContext();
	}
	catch (const std::invalid_argument& e)
	{
		throw InvalidContext(directive, expected);
	}

	if (context != expected)
	{
		throw InvalidContext(directive, expected);
	}
}

void	validateEnclosingContext(const Directive& directive,
								 const std::vector<Context::Context>& expected)
{
	Context::Context	context;
	try
	{
		context = directive.getContext();
	}
	catch (const std::invalid_argument& e)
	{
		throw InvalidContext(directive, expected);
	}

	if (std::find(expected.begin(),
				  expected.end(),
				  context) == expected.end())
				  // directive.getContext()) == allowedContexts.end())
				  // directive.enclosingContext) == allowedContexts.end())
	{
		throw InvalidContext(directive, expected);
	}
}

// TODO: right now, directives either allow duplicates or they don't.
//		 is there a situation where I need to allow a frequency of a certain
//		 number rather than just 0 or 1?
void	validateDuplicateDirective(const Directive& currDeclaration,
								   const Directive::Map& mappings)
{
	Directive::EqualRange	range = mappings.equal_range(currDeclaration.name);
	if (range.first == mappings.end())
	{
		return ;
	}
	size_t	count = std::distance(range.first, range.second);
	if (count > 0)
	{
		const Directive&	prevDeclaration = *range.first->second;
		throw DuplicateDirective(prevDeclaration, currDeclaration);
	}
}

/*
Syntax : prefix absolute_path;
Default: —
Context: global
Count  : 1 */
void	validatePrefix(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, Context::GLOBAL);
	validateDuplicateDirective(directive, mappings);

	const Parameter&	path = directive.parameters[0];
	if (path[0] != '/')
	{
		throw InvalidParameter(directive, path, "should be an absolute path");
	}
}

// TODO: accept more options for listen. right now, the only parameter
// accepted is a single port number

/*
Syntax : listen port;
Default: —
Context: server */
void	validateListen(const Directive& directive, const Directive::Map&)
{
	validateParameterSize(directive, 1);
	const String&	str = directive.parameters[0].value;
	int	portNum = str.toInt();

	if (portNum < 0 || portNum > 65535)
	{
		throw InvalidParameter(directive, directive.parameters[0],
							   "port number should be between 0-65535");
	}
	validateEnclosingContext(directive, Context::SERVER);
}

/*
Syntax : http { ... }
Default: —
Context: main
Count  : 1 */
void	validateHTTP(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 0);
	validateEnclosingContext(directive, Context::GLOBAL);
	validateDuplicateDirective(directive, mappings);
}

/*
Syntax : server { ... }
Default: —
Context: http */
void	validateServer(const Directive& directive, const Directive::Map&)
{
	validateEnclosingContext(directive, Context::HTTP);
}

/*
Syntax : server_name name;
Default : —
Context: server
*/
void	validateServerName(const Directive& directive, const Directive::Map&)
{
	validateParameterSize(directive, 1, std::numeric_limits<size_t>::max());
	validateEnclosingContext(directive, Context::SERVER);
}

/*
Syntax : location uri { ... }
Default: —
Context: server, location */
void	validateLocation(const Directive& directive, const Directive::Map&)
{
	// TODO: support exact matches?
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive,
							 vector_of(Context::SERVER)(Context::LOCATION));
}

/*
Syntax : root path;
Default: root html;
Context: http, server, location, if in location */
void	validateRoot(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive,
							 vector_of(Context::HTTP)
									  (Context::SERVER)
									  (Context::LOCATION));
	validateDuplicateDirective(directive, mappings);
}

/*
Syntax : index file ...;
Default: index index.html;
Context: http, server, location */
void	validateIndex(const Directive& directive, const Directive::Map& mappings)
{
	validateEnclosingContext(directive,
							 vector_of(Context::HTTP)
							 		  (Context::SERVER)
							 		  (Context::LOCATION));
	validateParameterSize(directive, 1, std::numeric_limits<size_t>::max());
	validateDuplicateDirective(directive, mappings);
}

/*
Syntax : types filename;
Default: —
Context: http, server, location */
void	validateTypes(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	// TODO(kecheong): allow server/location contexts
	// validateEnclosingContext(dir, HTTP);
	validateEnclosingContext(directive,
	                        (Context::HTTP,
	                         Context::SERVER,
	                         Context::LOCATION));
	validateDuplicateDirective(directive, mappings);
}

void	validateBoolean(const Directive& directive, const Parameter& parameter)
{
	if (parameter.value != "on" && parameter.value != "off")
	{
		throw InvalidParameter(directive, parameter, "expected `on` or `off`");
	}
}

/*
Syntax : autoindex boolean;
Default: off
Context: http, server, location */
void	validateAutoindex(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive,
							 vector_of(Context::HTTP)
									  (Context::SERVER)
									  (Context::LOCATION));
	validateDuplicateDirective(directive, mappings);
	validateBoolean(directive, directive.parameters[0]);
	if (countFrequency("accept_uploads", mappings) > 0)
	{
		throw InvalidDirective(directive);
	}
}

/*
Syntax : ExecCGI boolean;
Default: off
Context: location */
void	validateExecCGI(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, Context::LOCATION);
	validateDuplicateDirective(directive, mappings);
	validateBoolean(directive, directive.parameters[0]);
}

void	validateFileExtension(const Directive& directive, const Parameter& parameter)
{
	if (parameter.value[0] != '.')
	{
		throw InvalidParameter(directive, parameter,
							   "expected file extension beginning with a .");
	}
}

/*
Syntax : CGI_script file-extension ... ;
Default: —
Context: server */
void	validateCGIScript(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1, std::numeric_limits<size_t>::max());
	validateEnclosingContext(directive, Context::SERVER);
	validateDuplicateDirective(directive, mappings);
	const std::vector<Parameter>&	parameters = directive.parameters;
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		validateFileExtension(directive, parameters[i]);
	}
}

/*
Syntax : allow_method methods ... ;
Default: —
Context: location */
void	validateAllowMethod(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1, 3);
	validateParameterValues(directive,
							vector_of<Parameter>("GET")("POST")("DELETE")("HEAD"));
	validateEnclosingContext(directive,
							 vector_of(Context::LOCATION)(Context::CGI_SCRIPT));
	validateDuplicateDirective(directive, mappings);
}

/*
Syntax : client_max_body_size size;
Default: 1m
Context: http, server, location */
void	validateClientMaxBodySize(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive,
							vector_of(Context::HTTP)(Context::SERVER)(Context::LOCATION));
	validateDuplicateDirective(directive, mappings);

	// validate size format
	const String&	size = directive.parameters[0];
	for (size_t i = 0; i < size.length() - 1; ++i)
	{
		if (!std::isdigit(size[i]))
		{
			/*throw InvalidParameter(size);*/
		}
	}
	if (!(std::isdigit(size.back()) ||
		  size.back() == 'k' || size.back() == 'K' ||
		  size.back() == 'm' || size.back() == 'M'))
	{
		/*throw InvalidParameter(size);*/
	}
}

void	validateErrorCode(const Directive& directive, const Parameter& code)
{
	static std::vector<Parameter>	errorCodes = vector_of<Parameter>
	("300")("301")("302")("303")("304")("305")("306")("307")("308")
	("400")("401")("402")("403")("404")("405")("406")("407")("408")("409")("410")
	("411")("412")("413")("414")("415")("416")("417")("418")("421")("422")("423")
	("424")("425")("426")("428")("429")("431")("451")
	("500")("501")("502")("503")("504")("505")("506")("507")("508")("510")("511");

	if (std::find(errorCodes.begin(),
				  errorCodes.end(), code) == errorCodes.end())
	{
		throw InvalidParameter(directive, code, "unsupported HTTP error code");
	}
}

void	validateAcceptUploads(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, Context::LOCATION);
	validateDuplicateDirective(directive, mappings);
	validateBoolean(directive, directive.parameters[0].value);
	if (countFrequency("autoindex", mappings) > 0)
	{
		throw InvalidDirective(directive);
	}
}

/*
Syntax : upload_directory file_path;
Default: —
Context: location */
void	validateUploadDirectory(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive,
							 vector_of(Context::LOCATION)(Context::CGI_SCRIPT));
	validateDuplicateDirective(directive, mappings);
}

/*
Syntax : error_page code ... uri;
Default: —
Context: server */
void	validateErrorPage(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 2, std::numeric_limits<size_t>::max());
	validateEnclosingContext(directive, vector_of(Context::HTTP)(Context::SERVER)(Context::LOCATION));
	validateDuplicateDirective(directive, mappings);

	const std::vector<Parameter>&	parameters = directive.parameters;
	for (size_t i = 0; i < parameters.size()-1; ++i)
	{
		validateErrorCode(directive, parameters[i]);
	}
	// TODO: validate file format
}

/*
Syntax : CGI_directory directory;
Default: —
Context: CGI_script */
void	validateCGIDirectory(const Directive& dir, const Directive::Map& mappings)
{
	validateParameterSize(dir, 1);
	validateDuplicateDirective(dir, mappings);
}
