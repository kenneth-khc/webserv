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

#include <arpa/inet.h>
#include "Server.hpp"
#include "Client.hpp"

// TODO: limit request size based on config file. now defaulted to 1 mb
const size_t	Client::MAX_REQUEST_SIZE = 1 * 1024 * 1024;

Client::Client():
server(NULL),
messageBuffer(),
firstDataRecv(false),
lastActive(Time::getTimeSinceEpoch())
{
	messageBuffer.resize(1024);
}

Client::Client(Socket* socket, const Socket* receivedBy):
socket(socket),
receivedBy(receivedBy),
server(NULL),
messageBuffer(),
message(),
request(),
firstDataRecv(false),
lastActive(Time::getTimeSinceEpoch())
{
	messageBuffer.resize(1024);
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
