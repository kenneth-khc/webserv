/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:34:17 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/08 19:59:32 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "String.hpp"
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>

Socket::Socket() { };

Socket::Socket(String address, int portNum):
info(NULL),
portNum(portNum)
{
	addrinfo	requirements = addrinfo();
	requirements.ai_family = AF_INET;
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	(void)!getaddrinfo(address.c_str(), String::from<int>(portNum).c_str(),
				 &requirements, &info);

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

Socket::Socket(sockaddr_storage acceptedAddress):
info(NULL),
portNum(portNum)
{
	fillAddress(acceptedAddress);

	addrinfo	requirements = addrinfo();
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

	int	retval = getaddrinfo("127.0.0.1", String::from(port).c_str(),
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

Client	Socket::acceptNewConnection() const
{

}

String	Socket::getIPAddr() const
{
	String		ip;
	sockaddr*	address = info->ai_addr;

	if (address->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(address);
		char			ipv4[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr->sin_addr, ipv4, INET_ADDRSTRLEN);
		ip = ipv4;
	}
	else if (address->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(address);
		char			ipv6[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &addr->sin6_addr, ipv6, INET6_ADDRSTRLEN);
		ip = ipv6;
	}
	return (ip);
}

String	Socket::getPortNum() const
{
	unsigned short	portNum = 0;
	sockaddr*		address = info->ai_addr;

	if (address->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(address);
		portNum = ntohs(addr->sin_port);
	}
	else if (address->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(address);
		portNum = ntohs(addr->sin6_port);
	}
	return String::from<int>(portNum);
}

void	Socket::fillAddress(sockaddr_storage add)
{
	sockaddr*	address = (sockaddr*)&add;

	if (address->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(address);
		char			ipv4[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &addr->sin_addr, ipv4, INET_ADDRSTRLEN);
		ip = ipv4;
		port = ntohs(addr->sin_port);
	}
	else if (address->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(address);
		char			ipv6[INET6_ADDRSTRLEN];

		inet_ntop(AF_INET6, &addr->sin6_addr, ipv6, INET6_ADDRSTRLEN);
		ip = ipv6;
		port = ntohs(addr->sin6_port);
	}
}

