/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:34:17 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/07 22:39:12 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "String.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>

Socket::Socket() { };

Socket::Socket(int portNum):
port(portNum)
{
	addrinfo	requirements = {};

	requirements.ai_family = AF_INET;
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	fd = socket(requirements.ai_family,
				requirements.ai_socktype | SOCK_NONBLOCK,
				0);
	if (fd == -1)
	{
		// TODO: error handling;
	}

	int	yes = 1;
	int	retval = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval == 1)
	{
		// TODO: error handling;
	}
}

Socket::~Socket()
{
	/*close(fd);*/
}

int	Socket::bind()
{
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};

	requirements.ai_family = AF_INET;
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	int	retval = getaddrinfo("localhost", String::from(port).c_str(),
							 &requirements, &localhost);
	if (retval == -1)
	{
		// TODO: ERROR HANDLING
	}

	return ::bind(fd, localhost->ai_addr, sizeof *localhost->ai_addr);
	// TODO: error handling;
}

int	Socket::bind(const struct sockaddr* addr, socklen_t len)
{
	return ::bind(fd, addr, len);
	// TODO: error handling;
}

int	Socket::listen(int connectionCount)
{
	return ::listen(fd, connectionCount);
	// TODO: error handling;
}
