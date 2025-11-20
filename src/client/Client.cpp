/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:40:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 17:28:44 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "Client.hpp"
#include "ClientHeaderTimer.hpp"
#include "Socket.hpp"

Client::Client():
socket(NULL),
receivedBy(NULL),
defaultServer(NULL),
message(),
timer(new ClientHeaderTimer()),
cgis(),
messageBuffer(),
request(),
requestQueue(),
responseQueue()
{
	messageBuffer.resize(1024);
	requestQueue.push_back(Request());
}

Client::Client(Socket* socket, const Socket* receivedBy):
socket(socket),
receivedBy(receivedBy),
defaultServer(NULL),
message(),
timer(new ClientHeaderTimer()),
cgis(),
messageBuffer(),
request(),
requestQueue(),
responseQueue()
{
	messageBuffer.resize(1024);
	requestQueue.push_back(Request());
}

Client::~Client()
{
	if (timer != 0)
	{
		delete timer;
	}
}

Client::Client(const Client& rhs) :
socket(rhs.socket),
receivedBy(rhs.receivedBy),
defaultServer(rhs.defaultServer),
message(rhs.message),
timer(0),
cgis(rhs.cgis),
messageBuffer(rhs.messageBuffer),
request(rhs.request),
requestQueue(rhs.requestQueue),
responseQueue(rhs.responseQueue)
{
	if (rhs.timer != 0)
	{
		timer = rhs.timer->clone();
	}
}

Client	&Client::operator=(const Client& rhs)
{
	if (this == &rhs)
		return *this;

	socket = rhs.socket;
	receivedBy = rhs.receivedBy;
	defaultServer = rhs.defaultServer;
	message = rhs.message;

	if (timer != 0)
	{
		delete timer;
	}
	timer = 0;
	if (rhs.timer != 0)
	{
		timer = rhs.timer->clone();
	}

	cgis = rhs.cgis;
	messageBuffer = rhs.messageBuffer;
	request = rhs.request;
	requestQueue = rhs.requestQueue;
	responseQueue = rhs.responseQueue;
	return *this;
}

ssize_t	Client::receiveBytes()
{
	ssize_t	bytes = recv(socket->fd, &messageBuffer[0], messageBuffer.size(), 0);

	if (bytes > 0)
	{
		message.append(&messageBuffer[0], bytes);
		if (timer->getType() == Timer::KEEP_ALIVE)
		{
			delete timer;
			timer = new ClientHeaderTimer();
		}
		else
		{
			timer->update();
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
