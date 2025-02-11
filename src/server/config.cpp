/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 01:23:47 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/11 18:00:04 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Configuring the server before booting it up */

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include "Server.hpp"
#include <stdlib.h>
#include <sstream>

template <typename Type>
std::string	toString(const Type& t)
{
	std::stringstream	ss;
	ss << t;
	return ss.str();
}

void	Server::startListening()
{
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};
	requirements.ai_family = AF_INET;
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	int	retval = getaddrinfo(hostName.data(), toString(portNum).data(),
				 &requirements, &localhost);
	if (retval != 0)
	{
	}
	else if (localhost == NULL)
	{
	}
	/*dbg::printAddrInfos(localhost);*/

	listenerSocketFD = socket(localhost->ai_family,
							  localhost->ai_socktype | SOCK_NONBLOCK, 0);
	if (listenerSocketFD == -1)
	{
	// TODO: do we throw exceptions?
	}
	int	yes = 1;
	retval = setsockopt(listenerSocketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval != 0)
	{
		perror("setsockopt");
	}
	retval = bind(listenerSocketFD, localhost->ai_addr, sizeof *localhost->ai_addr);
	if (retval != 0)
	{
		perror("bind");
	}
	retval = listen(listenerSocketFD, 1);
	if (retval != 0)
	{
	}
	freeaddrinfo(localhost);
}

void	Server::initEpoll()
{
	epollFD = epoll_create(1);
	if (epollFD == -1)
	{
	}

	epoll_event	event = {};
	event.events |= EPOLLIN;
	event.data.fd = listenerSocketFD;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, listenerSocketFD, &event);
}

