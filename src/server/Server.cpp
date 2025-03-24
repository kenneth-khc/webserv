/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/24 09:39:31 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ErrorCode.hpp"
#include "connection.hpp"
#include "Base64.hpp"
#include "CGI.hpp"
#include "VectorInitializer.hpp"
#include "Utils.hpp"
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

const unsigned int	Server::timeoutValue = 5;
const Location		Server::defaultLocation = Location();

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
						 .transform(toBool)
						 .value_or(false)),
	indexFiles(serverBlock.recursivelyLookup< std::vector<String> >("index")
						  .value_or(vector_of<String>("index.html"))),
	MIMEMappings("mime.types"),
	cgiScript(vector_of<String>("py")("php"))
{
	// TODO: dynamic address
	const String&	address = "127.0.0.1";
	const String&	port = serverBlock.getParameterOf("listen")
									  .value_or("8000");
	assignSocket(address, port, existingSockets);
	configureLocations(serverBlock);
}

Response	Server::handleRequest(Request& request)
{
	Response	response;

	response.insert("Server", "42webserv");
	request.parseCookieHeader();
	processCookies(request, response);
	// match location
	const Location*	location = matchURILocation(request)
							  .value_or(&Server::defaultLocation);
	request.resolvedPath = location->root + request.path;
	try
	{
		if (request.method == "GET" || request.method == "HEAD")
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
	}
	catch (const ErrorCode& e)
	{
		response = e;
	}
	constructConnectionHeader(request, response);
	response.insert("Date", Time::printHTTPDate());
	return response;
}

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

	if (cookies.find("sid") == cookies.end())
	{
		String	sid = Base64::encode(Time::printHTTPDate());
		cookies.insert(std::make_pair("sid", Cookie("sid", sid)));
		response.insert("Set-Cookie", "sid=" + sid);
	}
	if (cookies.find("lang") == cookies.end())
	{
		cookies.insert(std::make_pair("lang", Cookie("lang", "en")));
		response.insert("Set-Cookie", "lang=en");
	}
}

void	Server::cgi(Response &response, const Request &request) const {
	CGI	cgi(*this, request);

	cgi.execute();
	cgi.parseOutput(response);
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

