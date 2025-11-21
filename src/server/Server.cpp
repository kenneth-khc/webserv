/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/21 09:43:14 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "ErrorCode.hpp"
#include "Socket.hpp"
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
	locations(),
	root(),
	autoindex(true),
	MIMEMappings("mime.types")
{}

Server::Server(const Directive& serverBlock,
			   std::map<int,Socket>& existingSockets):
// TODO(kecheong): allow listen to more than one socket for each server
socket(),
domainNames(serverBlock.getParametersOf("server_name")
					   .value_or(std::vector<String>())),
locations(),
root(serverBlock.recursivelyLookup<String>("root")
				.value_or("html")),
indexFiles(serverBlock.recursivelyLookup< std::vector<String> >("index")
					  .value_or(vector_of<String>("index.html"))),
autoindex(serverBlock.recursivelyLookup<String>("autoindex")
					 .transform(String::toBool)
					 .value_or(false)),
MIMEMappings(serverBlock.recursivelyLookup<String>("types")
						.value_or("mime.types")),
clientMaxBodySize(serverBlock.recursivelyLookup<String>("client_max_body_size")
							 .transform(String::toSize)
							 .value_or(1000000))
{
	const String&	listenParams = serverBlock.getParameterOf("listen")
												.value_or("0.0.0.0:8000");
	const String::size_type	colon = listenParams.find(':').value;
	const String&	address = listenParams.substr(0, colon);
	const String&	port = listenParams.substr(colon + 1);
	assignSocket(address, port, existingSockets);
	configureLocations(serverBlock);
	errorPages = serverBlock.generateErrorPagesMapping()
							.value_or(std::map<int,String>());
	Optional<Directive*>	cgiScriptBlock = serverBlock.getDirective("cgi_script");
	if (cgiScriptBlock.exists)
	{
		const std::vector<Parameter>&	extensions =
			cgiScriptBlock.value->getParameters();
		std::vector<String>	trimmed;
		for (size_t i = 0; i < extensions.size(); ++i)
		{
			trimmed.push_back(extensions[i].value.substr(1));
		}
	}
}

/*
	Once a well-formed request is received and parsed, a corresponding response
	will be generated.
*/
void	Server::handleRequest(Request& request, Response& response) const
{
	request.isSupportedVersion();

	request.location = const_cast<Location*>(matchURILocation(request)
											.value_or(&Server::defaultLocation));

	request.checkIfValidMethod();

	request.parseCookieHeader();
	processCookies(request, response);

	const String&	rootDir = pathHandler.resolveWithPrefix(request.location->root);
	request.resolvedPath = pathHandler.resolve(rootDir, request.decodedPath);

	// if (location->execCGI)
	// {
	// 	cgi(driver, client, response, request);
	// 	return ;
	// }

	if (request.method == "GET")
	{
		get(response, request);
	}
	else if (request.method == "POST")
	{
		post(response, request);
	}
	else if (request.method == "DELETE")
	{
		delete_(response, request);
	}

	constructConnectionHeader(request, response);
	response.insert("Date", Time::printHTTPDate());
	response.processStage |= Response::DONE;
}

Optional<const Location*>	Server::matchURILocation(const Request& request) const
{
	std::vector<Location>::const_iterator	longestMatch = locations.end();
	size_t									longestMatchSoFar = 0;
	for (std::vector<Location>::const_iterator it = locations.begin();
		 it != locations.end();
		 ++it)
	{
		const String&	location = it->uri;
		const String&	target = request.decodedPath;
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

void	Server::processCookies(Request& request, Response& response) const
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

void	Server::cgi(
	Driver& driver,
	Client& client,
	Response &response,
	Request &request) const
{
	CGI	*cgi = new CGI(client, request, response);

	cgi->generateEnv(driver.webServerName);
	cgi->execute(driver.epollFD, driver.cgis);
	client.cgis.push_back(cgi);
}

void	Server::assignSocket(const String& ip, const String& port,
							 std::map<int,Socket>& existingSockets)
{
	unsigned short	portNum = port.toInt();
	std::map<int, Socket>::iterator	iter =
		std::find_if(existingSockets.begin(),
					 existingSockets.end(),
					 Socket::IsMatchingAddress(ip, portNum));

	if (iter == existingSockets.end())
	{
		Socket	listener = Socket::spawn(ip, port);
		listener.bind();
		listener.listen(1024);
		existingSockets[listener.fd] = listener;
		socket = &existingSockets[listener.fd];
	}
	else
	{
		socket = &iter->second;
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
