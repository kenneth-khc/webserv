/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/16 20:15:16 by kecheong         ###   ########.fr       */
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
#include <sstream>
#include <string>
#include <vector>
#include "debugUtils.hpp"

void	error(const std::string& errmsg)
{
	std::cerr << errmsg << '\n';
	std::exit(1);
}

template <typename Type>
std::string	toString(Type const& t)
{
	std::stringstream	ss;
	ss << t;
	return ss.str();
}

/* Sets up a socket to start listening on a port,
 * returns the fd of the LISTENING socket */
int	startListening()
{
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	// TODO: port 80 requires the process to run with root permission
	int	retval = getaddrinfo("localhost", "8000", &requirements, &localhost);
	if (retval != 0)
	{
		error(gai_strerror(retval));
	}
	else if (localhost == NULL)
	{
		error("no valid address found");
	}
	dbg::printAddrInfos(localhost);

	int	sockFD = socket(localhost->ai_family, localhost->ai_socktype, 0);
	if (sockFD == -1)
	{
	// TODO: do we throw exceptions?
		error("socket failed");
	}
	int	yes = 1;
	retval = setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval != 0)
	{
		perror("setsockopt");
		error("setsockopt failed");
	}
	retval = bind(sockFD, localhost->ai_addr, sizeof *localhost->ai_addr);
	if (retval != 0)
	{
		perror("bind");
		error("bind failed");
	}
	retval = listen(sockFD, 1);
	if (retval != 0)
	{
		error("listen failed");
	}
	freeaddrinfo(localhost);
	return sockFD;
}

void	prettyPrintReceived(char* buf, ssize_t bytes, int clientFD)
{
	std::cout << "Received " << bytes << " bytes from " << clientFD << ":\n";
	for (int i = 0; i < bytes; ++i)
		std::cout << '-';
	std::cout << '\n';
	std::cout << buf;
	for (int i = 0; i < bytes; ++i)
		std::cout << '-';
	std::cout << '\n';
}

#if 0
void	communicate(int clientFD)
{
	char	readBuf[128] = {};
	std::string	buf;
	ssize_t	retval = recv(clientFD, readBuf, sizeof readBuf, 0);
	prettyPrintReceived(readBuf, retval);
	while (retval > 0)
	{
		std::memset(readBuf, 0, sizeof readBuf);
		retval = recv(clientFD, readBuf, sizeof readBuf, 0);
		if (retval == 0)
		{
			std::cout << "Client disconnected\n";
			break ;
		}
		prettyPrintReceived(readBuf, retval);
	}
}
#endif

void	server()
{
	int	listenerFD = startListening();
	sockaddr_storage	clientAddr = {};
	socklen_t			clientAddrLen = sizeof clientAddr;
	int					clientNum = 0;

	int	epoll = epoll_create(1);
	if (epoll == -1)
	{
		error("epoll_create failed");
	}
	epoll_event	registeredEvents = {};
	registeredEvents.events |= EPOLLIN;
	registeredEvents.data.fd = listenerFD;
	epoll_ctl(epoll, EPOLL_CTL_ADD, listenerFD, &registeredEvents);
	std::cout << "Added " << listenerFD << '\n';
	dbg::println("Server is running...");
	epoll_event*	readyEvents = new epoll_event[clientNum+1];
	for (int i = 0; i < clientNum+1; ++i)
	{
		std::memset(&readyEvents[i], 0, sizeof (epoll_event));
	}
	while (1)
	{
		dbg::println("Polling...");
		int	polled = epoll_wait(epoll, readyEvents, clientNum+1, -1);
		if (polled == -1)
		{
			perror("epw");
			error("epoll_wait failed");
		}
		std::cout << "Polled " << polled << '\n';
		for (int i = 0; i < polled; ++i)
		{
			if (readyEvents[i].data.fd == listenerFD)
			{
				dbg::println("Someone came in!");
				int	clientFD = accept(listenerFD, (sockaddr*)&clientAddr, &clientAddrLen);
				++clientNum;
				dbg::printSocketAddr((sockaddr*)&clientAddr);
				registeredEvents.events = EPOLLIN;
				registeredEvents.data.fd = clientFD;
				epoll_ctl(epoll, EPOLL_CTL_ADD, clientFD, &registeredEvents);
				std::string	tmp = "Hello, you are client " + toString(clientNum) + "!\n"
							+ "--------------------------------------------------\n";
				epoll_event*	oldEvents = readyEvents;
				readyEvents = new epoll_event[clientNum+1];
				for (int i = 0; i < clientNum; ++i)
				{
					readyEvents[i] = oldEvents[i];
				}
				readyEvents[clientNum].events |= EPOLLIN;
				readyEvents[clientNum].data.fd = clientFD;
				delete[] oldEvents;
				send(clientFD, tmp.data(), tmp.size(), 0);
			}
			else if (readyEvents[i].events & EPOLLIN)
			{
				char	readBuf[128] = {};
				std::string	buf;
				ssize_t	retval = recv(readyEvents[i].data.fd, readBuf, sizeof readBuf, 0);
				if (retval == 0)
				{
					std::cout << readyEvents[i].data.fd << " has closed the connection.\n";
					readyEvents[i].data.fd = -1;
				}
				else if (retval == -1)
				{
					perror("recv");
				}
				else
				{
					prettyPrintReceived(readBuf, retval, readyEvents[i].data.fd);
				}
			}
		}
	}
}
