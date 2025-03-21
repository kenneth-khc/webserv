/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 02:04:31 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include "ErrorCode.hpp"
#include "Server.hpp"
#include "connection.hpp"
#include "Base64.hpp"
#include "CGI.hpp"

const unsigned int	Server::timeoutValue = 5;

Server::Server():
	socket(),
	domainNames(),
	root(),
	defaultLocationConfig(),
	locations(),
	autoindex(true),
	MIMEMappings("mime.types")
{
	cgiScript.push_back("py");
	cgiScript.push_back("php");
}

Server::Server(std::vector<String> domainNames, Socket* socket):
	socket(socket),
	domainNames(domainNames),
	root(),
	defaultLocationConfig(),
	locations(),
	autoindex(true),
	MIMEMappings("mime.types")
{
	cgiScript.push_back("py");
	cgiScript.push_back("php");
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

Response	Server::handleRequest(Request& request)
{
	Response	response;

	response.insert("Server", "42webserv");
	request.parseCookieHeader();
	processCookies(request, response);
	// match location
	const Location*	location = matchURILocation(request)
							  .value_or(&defaultLocationConfig);
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

Optional<Location*>	Server::matchURILocation(const Request& request)
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
		return makeNone<Location*>();
	}
	else
	{
		return makeOptional(&*longestMatch);
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

