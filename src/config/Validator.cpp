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
#include "MissingDirective.hpp"
#include "String.hpp"
#include "InvalidParameter.hpp"
#include "InvalidParameterAmount.hpp"
#include "InvalidDirective.hpp"
#include "InvalidContext.hpp"
#include "DuplicateDirective.hpp"
#include "ServerNameConflict.hpp"
#include "DefaultServerNameConflict.hpp"
#include "VectorInitializer.hpp"
#include "host.hpp"
#include <iterator>
#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include <cctype>

Validator::Validator(ValidationFunc function):
function(function),
headerValidationFunc(NULL),
bodyValidationFunc(NULL)
{
}

Validator::Validator(ValidationFunc validateHeader, ValidationFunc validateBody):
function(validateHeader),
headerValidationFunc(validateHeader),
bodyValidationFunc(validateBody)
{
}

void	Validator::validateHeader(const Directive* directive,
								  const Directive::Map& mappings) const
{
	if (this->headerValidationFunc == NULL)
	{
		throw std::runtime_error("Missing header validation function for " +
								  directive->name);
	}
	return this->headerValidationFunc(*directive, mappings);
}

void	Validator::validateBody(const Directive* directive,
								  const Directive::Map& mappings) const
{
	if (this->bodyValidationFunc == NULL)
	{
		throw std::runtime_error("Missing body validation function for " +
								  directive->name);
	}
	return this->bodyValidationFunc(*directive, mappings);
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

/*
Syntax : server { ... }
Default: —
Context: http */
void	validateServerHeader(const Directive& directive, const Directive::Map&)
{
	validateEnclosingContext(directive, Context::HTTP);
	validateParameterSize(directive, 0);
}

std::set<Parameter>	getCommonListens(const Directive& serverA, const Directive& serverB)
{
	std::set<Parameter>	a;
	std::vector<Directive*>	listenA = serverA.getDirectives("listen");
	for (size_t i = 0; i < listenA.size(); ++i)
	{
		a.insert(listenA[i]->getParameter());
	}
	std::set<Parameter>	b;
	std::vector<Directive*>	listenB = serverB.getDirectives("listen");
	for (size_t i = 0; i < listenB.size(); ++i)
	{
		b.insert(listenB[i]->getParameter());
	}

	std::set<Parameter>	commonListens;
	std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
						  std::inserter(commonListens, commonListens.begin()));
	return commonListens;
}

std::set<Parameter>	getCommonServerNames(const Directive& serverA, const Directive& serverB)
{
	std::set<Parameter>	a;
	std::vector<Directive*>	serverNameA = serverA.getDirectives("server_name");
	for (size_t i = 0; i < serverNameA.size(); ++i)
	{
		const std::vector<Parameter>&	names = serverNameA[i]->getParameters();
		for (size_t j = 0; j < names.size(); ++j)
		{
			a.insert(names[j]);
		}
	}
	std::set<Parameter>	b;
	std::vector<Directive*>	serverNameB = serverB.getDirectives("server_name");
	for (size_t i = 0; i < serverNameB.size(); ++i)
	{
		const std::vector<Parameter>&	names = serverNameB[i]->getParameters();
		for (size_t j = 0; j < names.size(); ++j)
		{
			b.insert(names[j]);
		}
	}

	std::set<Parameter>	commonServerNames;
	std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
						  std::inserter(commonServerNames, commonServerNames.begin()));
	return commonServerNames;
}

void	validateServerBody(const Directive& directive, const Directive::Map&)
{
	validateEnclosingContext(directive, Context::HTTP);

	const Directive&		parent = *directive.parent;
	std::vector<Directive*>	servers = parent.getDirectives("server");

	for (size_t i = 0; i < servers.size(); ++i)
	{
		const Directive&	otherServer = *servers[i];
		std::set<Parameter> commonListens = getCommonListens(otherServer, directive);
		if (!commonListens.empty())
		{
			if (otherServer.getDirectives("server_name").empty() &&
				directive.getDirectives("server_name").empty())
			{
				throw DefaultServerNameConflict(otherServer, directive,
												commonListens.begin()->value);
			}
			std::set<Parameter>	commonServerNames =
				getCommonServerNames(otherServer, directive);
			if (!commonServerNames.empty())
			{
				throw ServerNameConflict(otherServer, directive,
				                         commonListens.begin()->value,
				                         commonServerNames.begin()->value);
			}
		}
	}
}

/*
Syntax : http { ... }
Default: —
Context: global 
Count  : 1 */
void	validateHttpHeader(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 0);
	validateEnclosingContext(directive, Context::GLOBAL);
	validateDuplicateDirective(directive, mappings);
}

void	validateHttpBody(const Directive& directive, const Directive::Map&)
{
	if (!directive.getDirective("server").exists)
	{
		throw MissingDirective(directive, "server");;
		throw std::runtime_error("missing server");
	}
}

/*
Syntax : location uri { ... }
Default: —
Context: server, location */
void	validateLocationHeader(const Directive& directive, const Directive::Map&)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive,
							 vector_of(Context::SERVER)(Context::LOCATION));
	
}

void	validateLocationBody(const Directive& directive, const Directive::Map&)
{
	validateEnclosingContext(directive,
							 vector_of(Context::SERVER)(Context::LOCATION));
}

/*
Syntax : listen address:port;
Default: —
Context: server */
#define MAX_PORTS 0xFFFF
void	validateListen(const Directive& directive, const Directive::Map&)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, Context::SERVER);

	const Parameter&	param = directive.parameters[0];
	const String&		str = param.value;

	Optional<String::size_type>	colon = str.find(":");
	if (!colon.exists)
	{
		throw InvalidParameter(directive,
							   param,
							   "invalid socket address",
							   "enter a socket address of IP:port");
	}
	const String&	ip = str.substr(0, colon.value);
	if (!isIPV4(ip))
	{
		throw InvalidParameter(directive,
							   param,
							   "invalid IPv4 address",
							   "enter a valid IP address in dotted-decimal format (x.x.x.x)");
	}
	const String&	port = str.substr(colon.value + 1);
	Optional<int>	portNum = String::parseInt(port);
	if (!portNum.exists || portNum.value < 0 || portNum.value > MAX_PORTS)
	{
		Diagnostic	diagnostic(param.diagnostic);
		diagnostic.colNum += ip.size() + 1;
		throw InvalidParameter(directive, diagnostic,
							   "invalid port number",
							   "port number can only be between 0-65535");
		
	}
}

/*
Syntax : server_name name ...;
Default : —
Context: server */
void	validateServerName(const Directive& directive, const Directive::Map&)
{
	validateParameterSize(directive, 1, std::numeric_limits<size_t>::max());
	validateEnclosingContext(directive, Context::SERVER);
}

/*
Syntax : root path;
Default: root html;
Context: http, server, location */
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
	validateEnclosingContext(directive, Context::HTTP);
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

void	validateFileExtension(const Directive& directive, const Parameter& parameter)
{
	if (parameter.value[0] != '.')
	{
		throw InvalidParameter(directive, parameter,
							   "expected file extension beginning with a .");
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
							vector_of<Parameter>("GET")("POST")("DELETE"));
	validateEnclosingContext(directive,
							 vector_of(Context::LOCATION)(Context::CGI_SCRIPT));
	validateDuplicateDirective(directive, mappings);
}

/*
Syntax : redirect code uri;
Default: —
Context: location */
void	validateRedirect(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 2);
	validateEnclosingContext(directive, Context::LOCATION);
	validateDuplicateDirective(directive, mappings);

	const String&	redirectCode = directive.parameters[0].value;
	if (!(redirectCode == "301" || redirectCode == "302"))
	{
		throw InvalidParameter(directive,
							   directive.parameters[0],
							   "invalid HTTP redirection code",
							   "set to 301 or 302, the only redirection codes we currently support");
	}
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

	const String&	size = directive.parameters[0];
	Diagnostic	diagnostic(directive.parameters[0].diagnostic);
	for (size_t i = 0; i < size.length() - 1; ++i)
	{
		if (!std::isdigit(size[i]))
		{
			throw InvalidParameter(directive,
								   diagnostic,
								   "unexpected character",
								   "specify size with only numbers or with a suffix k/K/m/M");
		}
		diagnostic.colNum++;
	}
	if (!(std::isdigit(size.back()) ||
		  size.back() == 'k' || size.back() == 'K' ||
		  size.back() == 'm' || size.back() == 'M'))
	{
		diagnostic.colNum = directive.parameters[0].diagnostic.colNum +
							size.size() - 1;
		throw InvalidParameter(directive,
							   diagnostic,
							   "unexpected size unit",
							   "specify kilobytes with k/K or megabytes with m/M");
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

void	validateUploadDirectory(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, vector_of(Context::LOCATION)(Context::CGI_SCRIPT));
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
Syntax : cgi_script file-extension ... ;
Default: —
Context: server */
void	validateCgiScriptHeader(const Directive& directive, const Directive::Map& mappings)
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

void	validateCgiScriptBody(const Directive& directive, const Directive::Map& mappings)
{
	validateCgiScriptHeader(directive, mappings);
	if (!directive.getDirective("cgi_bin_directory").exists)
	{
		throw MissingDirective(directive, "cgi_bin_directory");
	}
}

/*
Syntax : cgi_bin_directory directory;
Default: —
Context: cgi_script */
void	validateCgiBinDirectory(const Directive& directive, const Directive::Map&)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, vector_of(Context::CGI_SCRIPT));
}

/*
Syntax : cgi_upload_directory directory;
Default: —
Context: cgi_script */
void	validateCgiUploadDirectory(const Directive& directive, const Directive::Map& mappings)
{
	validateParameterSize(directive, 1);
	validateEnclosingContext(directive, Context::CGI_SCRIPT);
	validateDuplicateDirective(directive, mappings);
}

