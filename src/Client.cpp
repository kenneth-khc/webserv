/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:40:53 by kecheong          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2025/03/28 17:17:49 by cteoh            ###   ########.fr       */
=======
/*   Updated: 2025/03/29 21:43:09 by cteoh            ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "Server.hpp"
#include "Client.hpp"

// TODO: limit request size based on config file. now defaulted to 1 mb
const size_t	Client::MAX_REQUEST_SIZE = 1 * 1024 * 1024;

Client::Client():
<<<<<<< Updated upstream
server(NULL),
=======
timer(Client::CLIENT_HEADER),
address(),
addressLen(),
>>>>>>> Stashed changes
messageBuffer(),
keepAliveTimeout(0),
clientHeaderTimeout(0),
clientBodyTimeout(0)
{
	messageBuffer.resize(1024);
	requestQueue.push_back(Request());

	if (Server::clientHeaderTimeoutDuration > 0)
	{
		clientHeaderTimeout = Time::getTimeSinceEpoch() + Server::clientHeaderTimeoutDuration;
	}
	if (Server::clientBodyTimeoutDuration > 0)
	{
		clientBodyTimeout = Time::getTimeSinceEpoch() + Server::clientBodyTimeoutDuration;
	}
	if (Server::keepAliveTimeoutDuration > 0)
	{
		keepAliveTimeout = Time::getTimeSinceEpoch() + Server::keepAliveTimeoutDuration;
	}
}

Client::Client(Socket* socket, const Socket* receivedBy):
socket(socket),
receivedBy(receivedBy),
server(NULL),
message(),
messageBuffer(),
request(),
timer(Client::CLIENT_HEADER),
keepAliveTimeout(0),
clientHeaderTimeout(0),
clientBodyTimeout(0)
{
	messageBuffer.resize(1024);
	requestQueue.push_back(Request());

	if (Server::clientHeaderTimeoutDuration > 0)
	{
		clientHeaderTimeout = Time::getTimeSinceEpoch() + Server::clientHeaderTimeoutDuration;
	}
	if (Server::clientBodyTimeoutDuration > 0)
	{
		clientBodyTimeout = Time::getTimeSinceEpoch() + Server::clientBodyTimeoutDuration;
	}
	if (Server::keepAliveTimeoutDuration > 0)
	{
		keepAliveTimeout = Time::getTimeSinceEpoch() + Server::keepAliveTimeoutDuration;
	}
}

Client::Client(const Client& rhs) :
socket(rhs.socket),
receivedBy(rhs.receivedBy),
server(rhs.server),
message(rhs.message),
messageBuffer(rhs.messageBuffer),
request(rhs.request),
timer(rhs.timer),
keepAliveTimeout(rhs.keepAliveTimeout),
clientHeaderTimeout(rhs.clientHeaderTimeout),
clientBodyTimeout(rhs.clientBodyTimeout)
{}

ssize_t	Client::receiveBytes()
{
	ssize_t	bytes = recv(socket->fd, &messageBuffer[0], messageBuffer.size(), 0);

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
			clientHeaderTimeout = Time::getTimeSinceEpoch() + Server::clientHeaderTimeoutDuration;
		}
		if (timer & Client::CLIENT_BODY)
		{
			clientBodyTimeout = Time::getTimeSinceEpoch() + Server::clientBodyTimeoutDuration;
		}
	}
	return bytes;
}

ssize_t	Client::sendBytes(String &formattedResponse)
{
	ssize_t	bytes = send(socket->fd, formattedResponse.c_str(), formattedResponse.size(), 0);

	if (bytes > 0)
	{
		formattedResponse.erase(0, bytes);
	}
	return bytes;
}
