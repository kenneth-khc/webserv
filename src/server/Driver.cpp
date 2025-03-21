/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 02:39:17 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "Server.hpp"
#include "Configuration.hpp"
#include "ConfigErrors.hpp"
#include "Socket.hpp"
#include "contentLength.hpp"
#include "Directive.hpp"
#include "Utils.hpp"
#include "ErrorCode.hpp"
#include "VectorInitializer.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <iostream>
#include <algorithm>

const unsigned int	Driver::timeoutValue = 5;

Driver::Driver():
name("42webserv"),
epollFD(-1),
maxEvents(1),
readyEvents(NULL),
numReadyEvents(0),
MIMEMappings("mime.types"),
rootDir("root"),
pagesDir("pages"),
uploadsDir("uploads"),
miscPagesDir("misc_pages"),
cgiDir("cgi-bin"),
autoindex(true)
{
	readyEvents = new epoll_event[maxEvents];
	/*cgiScript.push_back("py");*/
	/*cgiScript.push_back("php");*/
}

// TODO: refactor this crap holy
void	Driver::configureFrom(const Configuration& config)
{
	epollFD = epoll_create(1);
	if (epollFD == -1){
		// TODO: error handling 
	}

	const Directive*		http = config.get("http");
	std::vector<Directive*>	serverDirectives = http->getDirectives("server");
	for (size_t i = 0; i < serverDirectives.size(); ++i)
	{
		const Directive*	serverDirective = serverDirectives[i];
		try
		{
			configNewServer(*serverDirective);
		}
		catch (const ConfigError& e)
		{
			std::cout << e.what() << '\n';
		}
	}
	epoll_event event = {};
	event.events |= EPOLLIN;

	for (std::map<int, Socket>::const_iterator it = listeners.begin();
		 it != listeners.end(); ++it)
	{
		event.data.fd = it->second.fd;
		int	retval = epoll_ctl(epollFD, EPOLL_CTL_ADD, event.data.fd, &event);
		if (retval == -1)
		{ //TODO: error handling
		}
	}
}

Socket*	Driver::spawnSocket(const String& ip, const String& port)
{
	Socket*	socket = NULL;
	int		portNum = to<int>(port);

	if (listeners.find(portNum) == listeners.end())
	{
		Socket	listener = Socket::spawn(ip, port);
		listener.bind();
		// TODO: should listen to more than 1?
		listener.listen(1);
		listeners[listener.fd] = listener;
		socket = &listeners[listener.fd];
	}
	else
	{
		std::map<int,Socket>::iterator	it;
		it = std::find_if(listeners.begin(),
						  listeners.end(),
						  IsMatchingPort(portNum));
		socket = &it->second;
	}
	return socket;
}

void	Driver::configNewServer(const Directive& directive)
{
	//TODO: dynamic address
	String	address = "127.0.0.1";
	String	port = directive.getParams<String>("listen").value_or("8000");
	Socket*	socket = spawnSocket(address, port);

	std::vector<String>	domainNames = directive
									 .getParams< std::vector<String> >("server_name")
									 .value_or(std::vector<String>());

	Server	newServer(domainNames, socket);
	newServer.root = directive.recursivelyLookup<String>("root")
							  .value_or("html");
	newServer.indexFiles = directive.recursivelyLookup< std::vector<String> >("index")
									.value_or(vector_of<String>("index.html"));

	newServer.configureLocations(directive);

	servers.push_back(newServer);
}

int	Driver::epollWait()
{
	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, 1000);
	/*std::cout << "epoll_wait() returned with " << numReadyEvents*/
	/*		  << " ready event" << (numReadyEvents > 1 ? "s\n" : "\n");*/

	if (numReadyEvents == -1)
	{
		//TODO: error handling
	}
	else if (numReadyEvents > 0 && (readyEvents[0].events & EPOLLRDHUP))
	{
		//	Closes socket in cases where client-side closes the connection on their end.
		int	fd = readyEvents[0].data.fd;
		logger.logConnection(Logger::CLOSE, fd,
			clients[readyEvents[0].data.fd]);
		close(fd);
		epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, 0);
		clients.erase(clients.find(fd));
		establishedSockets.erase(establishedSockets.find(fd));
		numReadyEvents = 0;
	}
	return numReadyEvents;
}

void	Driver::addToEpoll(int fd, EPOLL_EVENTS events)
{
	epoll_event	ep = epoll_event();
	ep.events = events;
	ep.data.fd = fd;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &ep);
	// TODO: error handling
}

void	Driver::processReadyEvents()
{
	for (int i = 0; i < numReadyEvents; ++i)
	{
		const epoll_event&	ev = readyEvents[i];
		int	readyFD = ev.data.fd;

		std::map<int,Socket>::iterator	it = listeners.find(readyFD);
		if (it != listeners.end())
		{
			const Socket&	listener = it->second;
			Socket			clientSocket = listener.accept();
			int				fd = clientSocket.fd;

			establishedSockets[fd] = clientSocket;
			addToEpoll(fd, EPOLL_EVENTS(EPOLLIN | EPOLLRDHUP));

			Client	newClient(&establishedSockets[fd], &listener);
			std::vector<Server>::iterator	it =
			std::find_if(servers.begin(),
						 servers.end(),
						 isDefaultListener(&listener));
			newClient.server = &(*it);

			clients[fd] = newClient;

			logger.logConnection(Logger::ESTABLISHED, fd, newClient);
		}
		else if (ev.events & EPOLLIN)
		{
			Client&	client = clients[ev.data.fd];
			client.updateLastActive();
			receiveBytes(client);
		}
	}
}

void	Driver::processMessages()
{
	// TODO: get all ready clients instead of just the first one
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;
	request.client = &client;

	try
	{
		if (!request.requestLineFound && client.endOfRequestLineFound())
		{
			request.parseRequestLine(client.message);
			request.requestLineFound = true;
		}
		if (request.requestLineFound &&
			!request.headersFound &&
			client.endOfHeaderFound())
		{
			request.parseHeaders(client.message);
			request.headersFound = true;

			Optional<String>	contentLength = request["Content-Length"];
			Optional<String>	transferEncoding = request["Transfer-Encoding"];

			if (contentLength.exists && transferEncoding.exists)
			{
				request.headers.erase(Request::stringToLower("Content-Length"));
			}
			else if (!contentLength.exists && !transferEncoding.exists)
			{
				request.insert(Request::stringToLower("Content-Length"), 0);
			}
			else if (contentLength.exists && !transferEncoding.exists)
			{
				if (isContentLengthHeader(contentLength.value) == false)
					throw BadRequest400();
			}
			else if (!contentLength.exists && transferEncoding.exists)
			{
				transferEncoding.value = Request::stringToLower(transferEncoding.value);
				if (transferEncoding.value.find("chunked").exists == false)
					throw BadRequest400();
			}
		}
		if (request.requestLineFound &&
			request.headersFound &&
			!request.messageBodyFound)
		{
			request.parseMessageBody(client.message);
		}
		if (request.messageBodyFound)
		{
			readyRequests.push(request);
			logger.logRequest(request, client);
		}
	}
	catch (const ErrorCode &e)
	{
		Response	response;
		response.insert("Server", name);
		response = e;
		readyResponses.push(response);
	}
}

// TODO: this should probably also check that the port matches
Optional<Server*>	Driver::matchServerName(const String& hostname)
{
	for (std::vector<Server>::iterator it = servers.begin();
		 it != servers.end();
		 ++it)
	{
		const std::vector<String>&	serverNames = it->domainNames;
		if (std::find(serverNames.begin(), serverNames.end(), hostname) != serverNames.end())
		{
			Server*	server = &(*it);
			return makeOptional(server);
		}
	}
	return makeNone<Server*>();
}

void	Driver::processReadyRequests()
{
	while (!readyRequests.empty())
	{
		Request&	request = readyRequests.front();
		String		host = request.find< Optional<String> >("Host").value_or("");
		if (host.find(':'))
		{
			host = host.consumeUntil(":").value;
		}
		// this picks the configuration block to use depending on server_name
		// or defaulting back to first server with the matching port
		Server*	server = matchServerName(host)
						.value_or(request.client->server);

		/*Response	response = handleRequest(request);*/
		Response	response = server->handleRequest(request);

		readyResponses.push(response);
		readyRequests.pop();
	}
}

/*void	Driver::processCookies(Request& request, Response& response)*/
/*{*/
/*	std::map<String, Cookie>&	cookies = request.cookies;*/
/**/
/*	if (cookies.find("sid") == cookies.end())*/
/*	{*/
/*		String	sid = Base64::encode(Time::printHTTPDate());*/
/*		cookies.insert(std::make_pair("sid", Cookie("sid", sid)));*/
/*		response.insert("Set-Cookie", "sid=" + sid);*/
/*	}*/
/*	if (cookies.find("lang") == cookies.end())*/
/*	{*/
/*		cookies.insert(std::make_pair("lang", Cookie("lang", "en")));*/
/*		response.insert("Set-Cookie", "lang=en");*/
/*	}*/
/*}*/

/*Response	Driver::handleRequest(Request& request)*/
/*{*/
/*	Response	response;*/
/**/
/*	response.insert("Server", name);*/
/*	request.parseCookieHeader();*/
/*	processCookies(request, response);*/
/*	try*/
/*	{*/
/*		if (request.method == "GET" || request.method == "HEAD")*/
/*		{*/
/*			get(response, request);*/
/*		}*/
/*		else if (request.method == "POST")*/
/*		{*/
/*			post(response, request);*/
/*		}*/
/*		else if (request.method == "DELETE")*/
/*		{*/
/*			delete_(response, request);*/
/*		}*/
/*	}*/
/*	catch (const ErrorCode& e)*/
/*	{*/
/*		response = e;*/
/*	}*/
/*	constructConnectionHeader(request, response);*/
/*	response.insert("Date", Time::printHTTPDate());*/
/*	return response;*/
/*}*/

void	Driver::generateResponses()
{
	while (!readyResponses.empty())
	{
		Client&		client = clients[readyEvents[0].data.fd];
		Response&	response = readyResponses.front();
		std::string	formattedResponse = response.toString();
		int			fd = client.socket->fd;

		send(fd, formattedResponse.c_str(), formattedResponse.size(), 0);
		logger.logResponse(response, client);

		if (response.flags & Response::CONNECTION_CLOSE)
		{
			close(fd);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, 0);
			establishedSockets.erase(establishedSockets.find(fd));
			clients.erase(clients.find(fd));
		}
		else
		{
			client.request = Request();
		}
		readyResponses.pop();
	}
}

void	Driver::monitorConnections()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		if (it->second.firstDataRecv == true && it->second.isTimeout() == true)
		{
			logger.logConnection(Logger::TIMEOUT, it->first, it->second);
			close(it->first);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, it->first, 0);
			establishedSockets.erase(establishedSockets.find(it->first));
			clients.erase(it++);
		}
		else
			it++;
	}
}

ssize_t	Driver::receiveBytes(Client& client)
{
	ssize_t	bytes = recv(client.socket->fd,
						 &client.messageBuffer[0],
						 client.messageBuffer.size(), 0);

	if (bytes > 0)
	{
		for (ssize_t i = 0; i < bytes; ++i)
		{
			client.message += client.messageBuffer[i];
		}
		if (client.firstDataRecv == false)
		{
			client.firstDataRecv = true;
		}
	}
	return bytes;
}

