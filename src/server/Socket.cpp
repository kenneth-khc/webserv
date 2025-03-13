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
#include <fcntl.h>

Socket::Socket() { };

Socket	Socket::spawn(const String& ip, const String& port)
{
	return Socket(ip, port);
}

Socket	Socket::wrap(int fd, sockaddr_storage address)
{
	return Socket(fd, address);
}

Socket::Socket(const String& ip, const String& port):
fd(-1),
ip(ip),
port(port),
portNum(port.toInt()),
_address(),
_addressLen(sizeof _address)
{
	addrinfo*	info = getAddrInfo(ip, port);
	while (info != NULL)
	{
		fd = socket(info->ai_family, info->ai_socktype, 0);
		if (fd != -1)
		{
			break ;
		}
		info = info->ai_next;
	}
	if (fd == -1)
	{
		// TODO: error handling;
	}
	_address = *(sockaddr_storage*)(info->ai_addr);
	fillAddress(_address);

	int	yes = 1;
	int	retval = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval == 1)
	{
		// TODO: error handling;
	}
	linger	linger = {.l_onoff = 1, .l_linger = 5};
	retval = setsockopt(fd, SOL_SOCKET, SO_LINGER, &linger, sizeof linger);
	if (retval == 1)
	{
		// TODO: error handling;
	}
	retval = fcntl(fd, F_SETFL, O_NONBLOCK);
	if (retval == 1)
	{
		// TODO: error handling;
	}
}

Socket::Socket(int fd, sockaddr_storage acceptedAddress):
fd(fd),
ip(),
port(),
portNum(),
_address(acceptedAddress),
_addressLen(sizeof _address)
{
	fillAddress(acceptedAddress);

	int	yes = 1;
	int	retval = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval == 1)
	{
		// TODO: error handling;
	}
	linger	linger = {.l_onoff = 1, .l_linger = 5};
	retval = setsockopt(fd, SOL_SOCKET, SO_LINGER, &linger, sizeof linger);
	if (retval == 1)
	{
		// TODO: error handling;
	}
	retval = fcntl(fd, F_SETFL, O_NONBLOCK);
	if (retval == 1)
	{
		// TODO: error handling;
	}
}

Socket::~Socket()
{
}

int	Socket::bind() const
{
	return ::bind(fd, (sockaddr*)&_address, _addressLen);
}

int	Socket::bind(const struct sockaddr* addr, socklen_t len) const
{
	return ::bind(fd, addr, len);
	// TODO: error handling;
}

int	Socket::listen(int connectionCount) const
{
	return ::listen(fd, connectionCount);
	// TODO: error handling;
}

Socket	Socket::accept() const
{
	sockaddr_storage	addr;
	socklen_t			addrLen;

	addrLen = static_cast<socklen_t>(sizeof addr);
	int	newFD = ::accept(this->fd, (sockaddr*)&addr, &addrLen);
	if (newFD == -1)
	{
		// TODO: error handling;
	}
	return Socket::wrap(newFD, addr);
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
	portNum = port.toInt();
}

addrinfo*	Socket::getAddrInfo(const String& ip, const String& port)
{
	addrinfo	requirements = {.ai_flags = AI_PASSIVE,
								.ai_family = AF_INET,
								.ai_socktype = SOCK_STREAM,
								.ai_protocol = 0,
								.ai_addrlen = 0,
								.ai_addr = NULL,
								.ai_canonname = NULL,
								.ai_next = NULL};
	addrinfo*	info = NULL;
	int	retval = ::getaddrinfo(ip.c_str(), port.c_str(), &requirements, &info);
	if (retval != 0)
	{
		// TODO: error handling
	}
	return info;
}

