/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/04 22:34:13 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include "utils.hpp"
#include "Server.hpp"

/* TODO: Configure the server based on the config file */
Server::Server():
map("mime.types"),
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
#include <iostream>
void	Server::epollWait()
{
	std::cout << "Polling...\n";
	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, -1);
	if (numReadyEvents == -1)
	{
		perror("epw");
		error("epoll_wait failed");
	}
	/*std::cout << "epoll_wait() returned with " << numReadyEvents*/
	/*		  << " ready event" << (numReadyEvents > 1 ? "s\n" : "\n");*/
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

	epoll_event	event;
	event.events = EPOLLIN;
	event.data.fd = client.socketFD;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, client.socketFD, &event);
	clients.insert(std::make_pair(client.socketFD, client));
}
