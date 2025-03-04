/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/04 18:59:41 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"
#include "Server.hpp"
#include "Configuration.hpp"
#include "ConfigErrors.hpp"
#include "Directive.hpp"
#include "Utils.hpp"
#include "connection.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <dirent.h>

Servers::Servers():
epollFD(-1),
maxEvents(1),
readyEvents(NULL),
numReadyEvents(0),
map("mime.types")
{
	readyEvents = new epoll_event[maxEvents];
}

// TODO: remove
void	die(const String& str)
{
	std::cerr << str << '\n';
	std::exit(1);
}

// TODO: beautify
std::pair<String,Directive>	findChild(const std::multimap<String,Directive>& children,
									  const String& key)
{
	return *children.find(key);
}

// TODO: beautify
std::vector<std::pair<String,Directive> >
findChildren(const std::multimap<String,Directive>&	allChildren,
			 const String& key)
{
	std::vector<std::pair<String,Directive> >	children;
	DirectiveRange	childRange = allChildren.equal_range(key);
	while (childRange.first != childRange.second)
	{
		children.push_back(*childRange.first);
		++childRange.first;
	}
	return children;
}

// TODO: refactor this crap holy
void	Servers::configureFrom(const Configuration& config)
{
	epollFD = epoll_create(1);
	if (epollFD == -1){
		// TODO: error handling 
	}

	const std::pair<String,Directive>	http = findChild(config.directives, "http");
	const Directive&	httpDirective = http.second;
	std::vector<std::pair<String,Directive> >	children = findChildren(httpDirective.directives, "server");
	for (size_t i = 0; i < children.size(); ++i)
	{
		const Directive&	server = children[i].second;
		std::cout << server.name << " : ";
		server.printParameters();
		std::cout << '\n';

		try
		{
			configNewServer(server);
		}
		catch (const ConfigError& e)
		{
			std::cout << e.what() << '\n';
		}
	}
	epoll_event event = {};
	event.events |= EPOLLIN;

	for (size_t i = 0; i < servers.size(); ++i)
	{
		event.data.fd = servers[i].socketFD;
		int	retval = epoll_ctl(epollFD, EPOLL_CTL_ADD, event.data.fd, &event);
		if (retval == -1)
			die("epoll error");
	}

}

void	Servers::configNewServer(const Directive& directive)
{
	Srvr	newServer;

	String	listenTo = directive.get("listen").value_or("8000");
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};
	requirements.ai_family = AF_INET;
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	int	retval = getaddrinfo("localhost", listenTo.c_str(),
				 &requirements, &localhost);
	if (retval == -1)
	{ // TODO: error handling
	}

	newServer.portNum = to<int>(listenTo);
	newServer.socketFD = socket(localhost->ai_family,
						  localhost->ai_socktype | SOCK_NONBLOCK, 0);
	if (newServer.socketFD == -1)
	{ // TODO: error handling
	}

	int	yes = 1;
	retval = setsockopt(newServer.socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval == -1)
	{ // TODO: error handling
	}
	 
	retval = bind(newServer.socketFD, localhost->ai_addr, sizeof *localhost->ai_addr);
	if (retval == -1)
	{ // TODO: error handling
	}
	
	retval = listen(newServer.socketFD, 1);
	if (retval != 0)
	{ // TODO: error handling
	}

	freeaddrinfo(localhost);

	// TODO: set up each server's sockets and stuff

	servers.push_back(newServer);
}

int	Servers::epollWait()
{
	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, 1000);

	std::cout << "epoll_wait() returned with " << numReadyEvents
			  << " ready event" << (numReadyEvents > 1 ? "s\n" : "\n");

	if (numReadyEvents == -1)
	{
		perror("epw");
	}
	else if (numReadyEvents > 0 && (readyEvents[0].events & EPOLLRDHUP))
	{
		//	Closes socket in cases where client-side closes the connection on their end.
		std::cout << "FD " << readyEvents[0].data.fd << " connection closed by client!\n";
		close(readyEvents[0].data.fd);
		epoll_ctl(epollFD, EPOLL_CTL_DEL, readyEvents[0].data.fd, 0);
		clients.erase(clients.find(readyEvents[0].data.fd));
		numReadyEvents = 0;
	}
	return numReadyEvents;
}

void	Servers::processReadyEvents()
{
	for (int i = 0; i < numReadyEvents; ++i)
	{
		const epoll_event&	ev = readyEvents[i];

		for (size_t j = 0 ; j < servers.size(); ++j)
		{
			if (ev.data.fd == servers[j].socketFD)
			{
				acceptNewClient(servers[j].socketFD);
			}
			else if (ev.events & EPOLLIN)
			{
				Client&	client = clients[ev.data.fd];
				client.updateLastActive();
				receiveBytes(client);
			}
		}
	}
}

static bool	endOfRequestLineFound(const std::string& message)
{
	return message.find("\r\n") != message.npos;
}

static bool	endOfHeaderFound(const std::string& message)
{
	return message.find("\r\n\r\n") != message.npos;
}

void	Servers::processMessages()
{
	/*if (listenerSocketFD == readyEvents[0].data.fd)*/
	/*	return ;*/
	/**/
	// TODO: get all ready clients instead of just the first one
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;

	if (!client.requestLineFound && endOfRequestLineFound(client.message))
	{
		request.parseRequestLine(client.message);
		client.requestLineFound = true;
	}
	if (client.requestLineFound &&
		!client.headersFound &&
		endOfHeaderFound(client.message))
	{
		request.parseHeaders(client.message);
		client.headersFound = true;
	}
	if (client.requestLineFound && client.headersFound)
	{
		Optional<int>	bodyLength = request.find< Optional<int> >("Content-Length");

		request.messageBody = client.message;
		if (client.message.size() == (size_t)bodyLength.value)
		{
			readyRequests.push(request);
			/*logger.logRequest(*this, request);*/
		}
	}

}

void	Servers::processReadyRequests()
{
	while (!readyRequests.empty())
	{
		Request&	request = readyRequests.front();
		Response	response = handleRequest(request);

		response.socketFD = request.socketFD;
		response.destAddress = request.srcAddress;
		readyResponses.push(response);
		readyRequests.pop();
	}
}

Response	Servers::handleRequest(const Request& request) const
{
	Response	response;

	/*const_cast<Request&>(request).method = POST;*/
	try
	{
		if (request.method == Request::GET)
		{
			get(response, request);
		}
		else if (request.method == Request::POST)
		{
			post(response, request);
		}
		else if (request.method == Request::PUT)
		{
			put(response, request);
		}
		else if (request.method == Request::DELETE)
		{
			delete_(response, request);
		}
		else if (request.method == Request::HEAD)
		{
			head(response, request);
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

void	Servers::generateResponses()
{
	while (!readyResponses.empty())
	{
		Client&		client = clients[readyEvents[0].data.fd];
		Response&	response = readyResponses.front();
		/*logger.logResponse(*this, response);*/

		std::string	formattedResponse = response.toString();
		send(response.socketFD, formattedResponse.c_str(), formattedResponse.size(), 0);

		if (response.flags & Response::CONNECTION_CLOSE)
		{
			close(response.socketFD);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, response.socketFD, 0);
			clients.erase(clients.find(response.socketFD));
		}
		else
		{
			client.reset();
		}
		readyResponses.pop();
	}
}

void	Servers::monitorConnections()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		if (it->second.firstDataRecv == true && it->second.isTimeout() == true)
		{
			std::cout << "FD " << it->first << " connection timeout!\n";
			close(it->first);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, it->first, 0);
			clients.erase(it++);
		}
		else
			it++;
	}
}

void	Servers::acceptNewClient(int socketFD)
{
	Client	client;

	client.addressLen = static_cast<socklen_t>(sizeof client.address);
	client.socketFD = accept(socketFD, (sockaddr*)&client.address,
							 &client.addressLen);
	fcntl(client.socketFD, F_SETFL, O_NONBLOCK);
	//++numClients;
	
	//	SO_LINGER prevents close() from returning when there's still data in
	//	the socket buffer. This avoids the "TCP reset problem" and allows
	//	graceful closure.
	linger	linger = {.l_onoff = 1, .l_linger = 5};
	setsockopt(client.socketFD, SOL_SOCKET, SO_LINGER, &linger, sizeof linger);

	epoll_event	event;
	event.events = EPOLLIN | EPOLLRDHUP;
	event.data.fd = client.socketFD;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, client.socketFD, &event);
	clients.insert(std::make_pair(client.socketFD, client));
}

ssize_t	Servers::receiveBytes(Client& client)
{
	ssize_t	bytes = recv(client.socketFD,
						 &client.messageBuffer[0],
						 client.messageBuffer.size(), 0);

	// TODO: fix where this belongs
	client.request.socketFD = client.socketFD;
	client.request.srcAddress = client.address;

	if (bytes > 0)
	{
		for (ssize_t i = 0; i < bytes; ++i)
		{
			client.message.push_back(client.messageBuffer[i]);
		}
		if (client.firstDataRecv == false)
		{
			client.firstDataRecv = true;
		}
	}
	return bytes;
}

void	Servers::generateDirectoryListing(Response& response, const std::string& dirName) const
{
	DIR*	dir = opendir(dirName.c_str());
	std::cout << dirName << '\n';
	if (dir)
	{
		response.messageBody += "<html>\n";
		response.messageBody += "<head><title>Index of " + dirName + "</title></head>\n";
		response.messageBody += "<body>\n";
		response.messageBody += "<h1>Index of " + dirName + "</h1>\n";
		response.messageBody += "<hr>\n";
		response.messageBody += "<pre>\n";

		dirent*	entry = readdir(dir);
		while (entry != NULL)
		{
			response.messageBody += "<a href=\"";
			response.messageBody += entry->d_name;
			response.messageBody += "\">";

			response.messageBody += entry->d_name;
			response.messageBody += "</a>\n";
			entry = readdir(dir);
		}
		response.messageBody += "</pre>\n";
		response.messageBody += "<hr>\n";
		response.messageBody += "</body>\n";
		response.messageBody += "</html>";
	}
	response.statusCode = 200;
	response.reasonPhrase = "OK";

}
