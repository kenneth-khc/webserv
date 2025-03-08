/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/07 19:28:05 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <iostream>
#include "Server.hpp"

const unsigned int	Server::timeoutValue = 5;

/* TODO: Configure the server based on the config file */
Server::Server():
name("42webserv"),
epollFD(-1),
hostName("localhost"),
portNum(8000),
listenerSocketFD(-1),
numClients(0),
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
	cgiScript.push_back("py");
	cgiScript.push_back("php");
}

Server::~Server()
{
	delete[] readyEvents;
}

int	Server::epollWait()
{
	// std::cout << "Polling...\n";
	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, 1000);

	// std::cout << "epoll_wait() returned with " << numReadyEvents
	// 		  << " ready event" << (numReadyEvents > 1 ? "s\n" : "\n");

	if (numReadyEvents == -1)
	{
		perror("epw");
	}
	else if (numReadyEvents > 0 && (readyEvents[0].events & EPOLLRDHUP))
	{
		//	Closes socket in cases where client-side closes the connection on their end.
		logger.logConnection(*this, Logger::CLOSE, readyEvents[0].data.fd,
			clients[readyEvents[0].data.fd]);
		close(readyEvents[0].data.fd);
		epoll_ctl(epollFD, EPOLL_CTL_DEL, readyEvents[0].data.fd, 0);
		clients.erase(clients.find(readyEvents[0].data.fd));
		numReadyEvents = 0;
	}
	return numReadyEvents;
}

void	Server::processReadyEvents()
{
	for (int i = 0; i < numReadyEvents; ++i)
	{
		const epoll_event&	ev = readyEvents[i];

		// TODO: look at multiple servers
		if (ev.data.fd == listenerSocketFD)
		{
			acceptNewClient();
		}
		else if (ev.events & EPOLLIN)
		{
			Client&	client = clients[ev.data.fd];
			client.updateLastActive();
			receiveBytes(client);
		}
	}
}

void	Server::acceptNewClient()
{
	Client	client;

	client.addressLen = static_cast<socklen_t>(sizeof client.address);
	client.socketFD = accept(listenerSocketFD, (sockaddr*)&client.address,
						  &client.addressLen);
	fcntl(client.socketFD, F_SETFL, O_NONBLOCK);
	++numClients;

	//	SO_LINGER prevents close() from returning when there's still data in
	//	the socket buffer. This avoids the "TCP reset problem" and allows
	//	graceful closure.
	struct linger	linger = {.l_onoff = 1, .l_linger = 5};
	setsockopt(client.socketFD, SOL_SOCKET, SO_LINGER, &linger, sizeof linger);

	epoll_event	event;
	event.events = EPOLLIN | EPOLLRDHUP;
	event.data.fd = client.socketFD;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, client.socketFD, &event);
	clients.insert(std::make_pair(client.socketFD, client));
	logger.logConnection(*this, Logger::ESTABLISHED, client.socketFD, client);
}

void	Server::monitorConnections()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		if (it->second.firstDataRecv == true && it->second.isTimeout() == true)
		{
			logger.logConnection(*this, Logger::TIMEOUT, it->first, it->second);
			close(it->first);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, it->first, 0);
			clients.erase(it++);
		}
		else
			it++;
	}
}
