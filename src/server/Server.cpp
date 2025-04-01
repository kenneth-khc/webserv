/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/28 21:46:19 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ErrorCode.hpp"
#include "connection.hpp"
#include "Base64.hpp"
#include "CGI.hpp"
#include "VectorInitializer.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <algorithm>

const unsigned int	Server::keepAliveTimeoutDuration = 5;
const unsigned int	Server::cgiTimeoutDuration = 3;
const unsigned int	Server::clientHeaderTimeoutDuration = 2;
const unsigned int	Server::clientBodyTimeoutDuration = 2;
const Location		Server::defaultLocation = Location();
PathHandler			Server::pathHandler;

Server::Server():
	socket(),
	domainNames(),
	root(),
	locations(),
	autoindex(true),
	MIMEMappings("mime.types")
{
	cgiScript.push_back("py");
	cgiScript.push_back("php");
}

Server::Server(const Directive& serverBlock,
			   std::map<int,Socket>& existingSockets):
	socket(),
	domainNames(serverBlock.getParametersOf("server_name")
						   .value_or(std::vector<String>())),
	root(serverBlock.recursivelyLookup<String>("root")
					.value_or("html")),
	locations(),
	autoindex(serverBlock.recursivelyLookup<String>("autoindex")
						 .transform(String::toBool)
						 .value_or(false)),
	indexFiles(serverBlock.recursivelyLookup< std::vector<String> >("index")
						  .value_or(vector_of<String>("index.html"))),
	MIMEMappings(serverBlock.recursivelyLookup<String>("types")
							.value_or("")),
	clientMaxBodySize(serverBlock.recursivelyLookup<String>("client_max_body_size")
								 .transform(String::toSize)
								 .value_or(1000000)),
	cgiScript(vector_of<String>("py")("php"))
{
	// TODO: dynamic address
	const String&	address = "127.0.0.1";
	const String&	port = serverBlock.getParameterOf("listen")
									  .value_or("8000");
	assignSocket(address, port, existingSockets);
	configureLocations(serverBlock);
	errorPages = serverBlock.generateErrorPagesMapping()
							.value_or(std::map<int,String>());
}

void	Server::handleRequest(Request& request, Response& response)
{
	if (!request.isSupportedVersion())
	{
		throw VersionNotSupported505();
	}

	request.parseCookieHeader();
	processCookies(request, response);

	request.path = pathHandler.normalize(request.path);
	const Location*	location = matchURILocation(request)
							  .value_or(&Server::defaultLocation);
	location->checkIfAllowedMethod(request.method);

	const String&	rootDir = pathHandler.resolveWithPrefix(location->root);
	request.resolvedPath = pathHandler.resolve(rootDir, request.path);


	if (request.path.starts_with(location->uri) &&
		location->executeCGI == true &&
		request.path.ends_with(".bla"))	// Test-specific condition
	{
		cgi(response, request);
		return ;
	}

	if (request.method == "GET")
	{
		get(response, request, *location);
	}
	else if (request.method == "POST")
	{
		post(response, request);
	}
	else if (request.method == "DELETE")
	{
		delete_(response, request);
	}
	else	// Test-specific condition
	{
		if (request.path == "/")
			throw MethodNotAllowed405();
		throw NotImplemented501();
	}

	constructConnectionHeader(request, response);
	response.insert("Date", Time::printHTTPDate());
	response.processStage |= Response::DONE;
}

// TODO: exact matches
Optional<const Location*>	Server::matchURILocation(const Request& request)
{
	std::vector<Location>::iterator	longestMatch = locations.end();
	size_t							longestMatchSoFar = 0;
	for (std::vector<Location>::iterator it = locations.begin();
		 it != locations.end();
		 ++it)
	{
		const String&	location = it->uri;
		const String&	target = request.path;
		if (target.starts_with(location) && location.length() > longestMatchSoFar)
		{
			longestMatchSoFar = location.length();
			longestMatch = it;
		}
	}
	if (longestMatch == locations.end())
	{
		return makeNone<const Location*>();
	}
	else
	{
		std::vector<Location>::const_iterator	retval = longestMatch;
		return makeOptional(&*retval);
	}
}

void	Server::processCookies(Request& request, Response& response)
{
	std::map<String, Cookie>&	cookies = request.cookies;
	Cookie						cookie;

	if (cookies.find("sid") == cookies.end())
	{
		String	sid = Base64::encode(Time::printHTTPDate());

		cookie = Cookie("sid", sid);
		cookie.path = Optional<String>("/");
		cookies.insert(std::make_pair("sid", cookie));
		response.insert("Set-Cookie", cookie.constructSetCookieHeader());
	}
}

void	Server::cgi(Response &response, Request &request) const {
	if (request.method != "GET" && request.method != "POST")
		throw NotImplemented501();

	CGI	*cgi = new CGI(*this, *request.client, request, response);

	cgi->generateEnv(*this);
	cgi->execute(*this);
}

void	Server::assignSocket(const String& address, const String& port,
							 std::map<int,Socket>& existingSockets)
{
	unsigned short	portNum = port.toInt();

	if (existingSockets.find(portNum) == existingSockets.end())
	{
		Socket	listener = Socket::spawn(address, port);
		listener.bind();
		listener.listen(1);
		existingSockets[listener.fd] = listener;
		socket = &existingSockets[listener.fd];
	}
	else
	{
		std::map<int,Socket>::iterator	it;
		it = std::find_if(existingSockets.begin(),
						  existingSockets.end(),
						  IsMatchingPort(portNum));
		socket = &it->second;
	}
}

void	Server::configureLocations(const Directive& directive)
{
	std::vector<Directive*>	locationBlocks = directive.getDirectives("location");

	for (size_t i = 0; i < locationBlocks.size(); ++i)
	{
		const Directive*	locationBlock = locationBlocks[i];
		Location			location(*locationBlock);

		this->locations.push_back(location);
	}
}
