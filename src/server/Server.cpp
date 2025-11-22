/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/23 01:57:27 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "Server.hpp"
#include "Defaults.hpp"
#include "Client.hpp"
#include "ErrorCode.hpp"
#include "Socket.hpp"
#include "connection.hpp"
#include "Base64.hpp"
#include "CGI.hpp"
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
sockets(),
domainNames(Defaults::SERVER_NAME),
locations(),
root(Defaults::ROOT),
indexFiles(Defaults::INDEX),
autoindex(Defaults::AUTOINDEX.toBool()),
MIMEMappings(Defaults::TYPES),
clientMaxBodySize(Defaults::CLIENT_MAX_BODY_SIZE.toSize())
{}

Server::Server(const Directive& block,
			   std::map<int,Socket>& existingSockets):
sockets(),
domainNames(block.getInherited("server_name", Defaults::SERVER_NAME)),
locations(configureLocations(block)),
root(block.getInherited("root", Defaults::ROOT)),
indexFiles(block.getInherited("index", Defaults::INDEX)),
autoindex(block.getInherited("autoindex", Defaults::AUTOINDEX).toBool()),
MIMEMappings(block.getInherited("types", Defaults::TYPES)),
clientMaxBodySize(block.getInherited("client_max_body_size", Defaults::CLIENT_MAX_BODY_SIZE).toSize())
{
	std::vector<Directive*> listens = block.getDirectives("listen");
	if (listens.empty())
	{
		assignSocket("0.0.0.0", "8000", existingSockets);
	}
	else
	{
		for (std::size_t i = 0; i < listens.size(); ++i)
		{
			const String&	listen = listens[i]->parameters[0];
			size_t			colon = listen.find(':').value;
			const String&	address = listen.substr(0, colon);
			const String&	port = listen.substr(colon + 1);
			assignSocket(address, port, existingSockets);
		}
	}
	errorPages = block.generateErrorPagesMapping()
							.value_or(std::map<int,String>());
	Optional<Directive*>	cgiScriptBlock = block.getDirective("cgi_script");
	if (cgiScriptBlock.exists)
	{
		const std::vector<Parameter>&	extensions =
			cgiScriptBlock.value->getParameters();
		std::vector<String>	trimmed;
		for (size_t i = 0; i < extensions.size(); ++i)
		{
			trimmed.push_back(extensions[i].value.substr(1));
		}
		cgiScripts = trimmed;

		Optional<Directive*>	cgiBinDirectoryDirective = block.getDirective("cgi_bin_directory");
		Optional<Directive*>	cgiUploadDirectoryDirective = block.getDirective("cgi_upload_directory");

		cgiBinDirectory = cgiBinDirectoryDirective.value->parameters[0];
		cgiUploadDirectory = cgiUploadDirectoryDirective.value->parameters[0];
	}
}

/*
	Once a well-formed request is received and parsed, a corresponding response
	will be generated.
*/
void	Server::handleRequest(
Driver& driver,
Client& client,
Request& request,
Response& response) const
{
	request.isSupportedVersion();

	request.location = const_cast<Location*>(matchURILocation(request)
											.value_or(&Server::defaultLocation));

	request.checkIfValidMethod();

	request.parseCookieHeader();
	processCookies(request, response);

	const String&	rootDir = pathHandler.resolveWithPrefix(request.location->root);
	request.resolvedPath = pathHandler.resolve(rootDir, request.decodedPath);

	if (request.location->shouldRedirect())
	{
		return request.location->executeRedirection(response);
	}

	if (cgiScripts.size() != 0 && cgiBinDirectory.starts_with(request.location->uri))
	{
		cgi(driver, client, response, request);
		return ;
	}

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
	CGI	*cgi = new CGI(cgiScripts, client, request, response);

	cgi->generateEnv(driver.webServerName);
	cgi->execute(driver.epollFD, driver.cgis);
	client.cgis.push_back(cgi);
}

void	Server::assignSocket(const String& ip, const String& port,
							 std::map<int,Socket>& listeners)
{
	unsigned short	portNum = port.toInt();
	std::map<int, Socket>::iterator	iter =
		std::find_if(listeners.begin(),
					 listeners.end(),
					 Socket::IsMatchingAddress(ip, portNum));

	if (iter == listeners.end())
	{
		Socket	listener = Socket::spawn(ip, port);
		listener.bind();
		listener.listen(1024);
		listeners[listener.fd] = listener;
		this->sockets.push_back(&listeners[listener.fd]);
	}
	else
	{
		this->sockets.push_back(&iter->second);
	}
}

std::vector<Location>	Server::configureLocations(const Directive& block) const
{
	std::vector<Location>	locations;
	std::vector<Directive*>	locationBlocks = block.getDirectives("location");

	for (size_t i = 0; i < locationBlocks.size(); ++i)
	{
		locations.push_back(*locationBlocks[i]);
	}
	return locations;
}
