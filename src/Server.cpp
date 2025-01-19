/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/19 17:53:09 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "debugUtils.hpp"
#include "utils.hpp"
#include "Server.hpp"
#include <unistd.h>

/* TODO: Configure the server based on the config file */
Server::Server():
epollFD(-1),
hostName("localhost"),
portNum(8000),
listenerSocketFD(-1),
numClients(0),
maxEvents(1),
readyEvents(NULL),
numReadyEvents(0) 
{
	readyEvents = new epoll_event[maxEvents];
}

Server::~Server()
{
	delete[] readyEvents;
}

void	Server::startListening()
{
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	int	retval = getaddrinfo(hostName.data(), toString(portNum).data(),
				 &requirements, &localhost);
	if (retval != 0)
	{
		error(gai_strerror(retval));
	}
	else if (localhost == NULL)
	{
		error("no valid address found");
	}
	dbg::printAddrInfos(localhost);

	listenerSocketFD = socket(localhost->ai_family, localhost->ai_socktype, 0);
	if (listenerSocketFD == -1)
	{
	// TODO: do we throw exceptions?
		error("socket failed");
	}
	int	yes = 1;
	retval = setsockopt(listenerSocketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval != 0)
	{
		perror("setsockopt");
		error("setsockopt failed");
	}
	retval = bind(listenerSocketFD, localhost->ai_addr, sizeof *localhost->ai_addr);
	if (retval != 0)
	{
		perror("bind");
		error("bind failed");
	}
	retval = listen(listenerSocketFD, 1);
	if (retval != 0)
	{
		error("listen failed");
	}
	freeaddrinfo(localhost);
}

void	Server::initEpoll()
{
	epollFD = epoll_create(1);
	if (epollFD == -1)
	{
		error("epoll_create failed");
	}

	epoll_event	event = {};
	event.events |= EPOLLIN;
	event.data.fd = listenerSocketFD;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, listenerSocketFD, &event);
	std::cout << "Added " << listenerSocketFD << '\n';
}

void	Server::epollWait()
{
	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, -1);
	if (numReadyEvents == -1)
	{
		perror("epw");
		error("epoll_wait failed");
	}
	std::cout << "epoll_wait() returned with " << numReadyEvents
			  << " ready events.\n";
}

void	Server::processReadyEvents()
{
	for (int i = 0; i < numReadyEvents; ++i)
	{
		epoll_event const&	ev = readyEvents[i];

		if (ev.data.fd == listenerSocketFD)
		{
			acceptNewClient();
		}
		else if (ev.events & EPOLLIN)
		{
			std::string	request = receiveRequest(ev.data.fd);
			std::cout << "Request:\n";
			dbg::println("---------------------------------");
			std::cout << request;
			dbg::println("---------------------------------");
			// TODO: generate responses for the client
			std::string	response =
			"HTTP/1.1 200 OK\r\n" \
			"Connection: close\r\n\r\n <html>HELLO WORLD</html>\r\n";
			send(ev.data.fd, response.data(), response.size(), 0);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, NULL);
			close(ev.data.fd);
		}
	}
}

void	Server::acceptNewClient()
{
	sockaddr_storage	clientAddr = {};
	socklen_t			clientAddrLen = sizeof clientAddr;
	epoll_event			event;

	dbg::println("Someone came in!");
	int	clientFD = accept(listenerSocketFD, (sockaddr*)&clientAddr, &clientAddrLen);
	++numClients;
	dbg::printSocketAddr((sockaddr*)&clientAddr);
	event.events = EPOLLIN;
	event.data.fd = clientFD;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, clientFD, &event);
}

std::string	Server::receiveRequest(int fd) const
{
	char		buf[1024] = {};
	std::string	request;
	int			retval;

	while ((retval = recv(fd, buf, sizeof buf-1, 0)) > 0)
	{
		buf[retval] = '\0';
		request += buf;
		if (request.find("\r\n\r\n") != request.npos)
			break ;
	}
	if (retval == -1)
	{
		perror("recv");
		error("recv failed");
	}
	if (retval == 0)
	{
		std::cout << "Client has closed the connection\n";
	}
	epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, NULL);
	return request;
}

