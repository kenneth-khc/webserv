/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:40:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/05 22:41:37 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Client.hpp"

// TODO: limit request size based on config file. now defaulted to 1 mb
const size_t	Client::MAX_REQUEST_SIZE = 1 * 1024 * 1024;

Client::Client():
address(),
addressLen(),
messageBuffer(),
firstDataRecv(false),
lastActive(Time::getTimeSinceEpoch())
{
	messageBuffer.resize(1024);
}

String	Client::getIPAddr() const
{
	String		ip;
	sockaddr*	client = (sockaddr*)&address;

	if (client->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(client);
		char			ipv4[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr->sin_addr, ipv4, INET_ADDRSTRLEN);
		ip = ipv4;
	}
	else if (client->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(client);
		char			ipv6[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &addr->sin6_addr, ipv6, INET6_ADDRSTRLEN);
		ip = ipv6;
	}
	return (ip);
}

String	Client::getPortNum() const
{
	sockaddr*		client = (sockaddr*)&address;
	unsigned short	portNum;
	if (client->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(client);
		portNum = ntohs(addr->sin_port);
	}
	else if (client->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(client);
		portNum = ntohs(addr->sin6_port);
	}

	std::stringstream	stream;
	stream << portNum;
	return (stream.str());
}

bool	Client::endOfRequestLineFound() const {
	return message.find("\r\n").exists;
}

bool	Client::endOfHeaderFound() const {
	return message.find("\r\n\r\n").exists;
}

bool	Client::isTimeout() const
{
	if (firstDataRecv == true
		&& (Time::getTimeSinceEpoch() - lastActive >= Server::timeoutValue))
		return (true);
	return (false);
}

void	Client::updateLastActive()
{
	lastActive = Time::getTimeSinceEpoch();
}
