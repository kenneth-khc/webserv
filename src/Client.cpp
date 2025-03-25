/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:40:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/26 19:00:33 by cteoh            ###   ########.fr       */
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
timer(Client::CLIENT_HEADER),
keepAliveTime(Time::getTimeSinceEpoch()),
clientHeaderTime(Time::getTimeSinceEpoch()),
clientBodyTime(Time::getTimeSinceEpoch())
{
	messageBuffer.resize(1024);
	requestQueue.push_back(Request());
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
	unsigned short	portNum = 0;
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

ssize_t	Client::receiveBytes()
{
	ssize_t	bytes = recv(socket.fd, &messageBuffer[0], messageBuffer.size(), 0);

	if (bytes > 0)
	{
		message.append(&messageBuffer[0], bytes);
		if (timer & Client::KEEP_ALIVE)
		{
			timer &= ~Client::KEEP_ALIVE;
			timer |= Client::CLIENT_HEADER;
		}
		if (timer & Client::CLIENT_HEADER)
		{
			clientHeaderTime = Time::getTimeSinceEpoch();
		}
		if (timer & Client::CLIENT_BODY)
		{
			clientBodyTime = Time::getTimeSinceEpoch();
		}
	}
	return bytes;
}

ssize_t	Client::sendBytes(String &formattedResponse)
{
	ssize_t	bytes = send(socket.fd, formattedResponse.c_str(), formattedResponse.size(), 0);

	if (bytes > 0)
	{
		formattedResponse.erase(0, bytes);
	}
	return bytes;
}
