/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/24 23:45:54 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "Server.hpp"
#include "Defaults.hpp"
#include "Client.hpp"
#include "Socket.hpp"
#include "connection.hpp"
#include "Base64.hpp"
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
const unsigned int	Server::cgiTimeoutDuration = 5;
const unsigned int	Server::clientHeaderTimeoutDuration = 5;
const unsigned int	Server::clientBodyTimeoutDuration = 5;
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
	std::vector<Directive*>	cgiScriptBlocks = block.getDirectives("cgi_script");
	if (!cgiScriptBlocks.empty())
	{
		for (std::vector<Directive*>::const_iterator it = cgiScriptBlocks.begin(); it != cgiScriptBlocks.end(); it++)
		{
			this->cgiScriptBlocks.push_back(*(*it));
		}
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
Response& response,
std::set<Timer*>& activeTimers
) const
{
	request.isSupportedVersion();

	request.location = const_cast<Location*>(matchURILocation(request)
											.value_or(&Location::defaultLocation()));

	if (request.path.find(".").exists)
	{
		CGI*	cgi = handleCGI(driver, client, request, response);

		if (cgi)
		{
			activeTimers.insert(cgi->timer);
			return ;
		}
	}

	request.isValidMethod(request.location->allowedMethods);
	request.isWithinBodySizeLimit(request.location->clientMaxBodySize);

	if (request.location->shouldRedirect())
	{
		request.location->executeRedirection(response);
		constructConnectionHeader(request, response);
		return;
	}

	request.parseCookieHeader();
	processCookies(request, response);

	const String&	rootDir = pathHandler.resolveWithPrefix(request.location->root);
	const String&	alias = request.location->alias;
	if (!alias.empty())
	{
		request.decodedPath= request.decodedPath.replace(0,
								request.location->uri.size(), alias);
		if (request.decodedPath[0] != '/')
		{
			request.decodedPath = "/" + request.decodedPath;
		}
	}
	request.resolvedPath = pathHandler.resolve(rootDir, request.decodedPath);

	if (request.method == "GET" || request.method == "HEAD")
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

CGI	*Server::handleCGI(
Driver& driver,
Client& client,
Request &request,
Response &response
) const
{
	Optional<String::size_type>	extPos = request.path.find(".");
	Optional<String::size_type>	pathInfoPos = request.path.find("/", extPos.value);
	String						extension;

	if (pathInfoPos.exists == true)
		extension = request.path.substr(extPos.value, pathInfoPos.value - extPos.value);
	else
		extension = request.path.substr(extPos.value);

	String										pathInfo;
	String										scriptName;
	std::vector<CGIScriptBlock>::const_iterator	block = cgiScriptBlocks.begin();
	bool										stop = false;

	while (block != cgiScriptBlocks.end()) {
		std::vector<String>::const_iterator cgiScript = block->cgiScripts.begin();

		while (cgiScript != block->cgiScripts.end()) {
			if (extension == ("." + *cgiScript)) {
				if (pathInfoPos.exists == true) {
					scriptName = request.path.substr(0, pathInfoPos.value - 1);
					pathInfo = request.path.substr(pathInfoPos.value);
				}
				else
					scriptName = request.path;
				stop = true;
			}
			if (stop)
				break ;
			cgiScript++;
		}
		if (stop)
			break ;
		block++;
	}

	if (block == cgiScriptBlocks.end())
		return (NULL);
	request.isValidMethod(block->allowedMethods);
	request.isWithinBodySizeLimit(block->clientMaxBodySize);

	if (pathInfo.length() == 0)
		pathInfo = "/";

	CGI	*cgi = new CGI(client, request, response, extension,
							pathInfo, scriptName, block);

	cgi->generateEnv(driver.webServerName);
	cgi->execute(driver.epollFD, driver.cgis);
	client.cgis.push_back(cgi);
	return (cgi);
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
