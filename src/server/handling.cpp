/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 04:05:29 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/02 05:56:50 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "Server.hpp"
#include "ErrorCode.hpp"

ssize_t	Server::receiveBytes(Client& client)
{
	ssize_t	bytes = recv(client.socketFD,
						 &client.messageBuffer[0],
						 client.messageBuffer.size(), 0);

	// TODO: fix where this belongs
	client.request.socketFD = client.socketFD;
	client.request.srcAddress = client.address;

	if (bytes > 0)
	{
		for (ssize_t i = 0; i < bytes; ++i)
		{
			client.message.push_back(client.messageBuffer[i]);
		}
	}
	return bytes;
}

static bool	endOfRequestLineFound(const std::string& message)
{
	return message.find("\r\n") != message.npos;
}

static bool	endOfHeaderFound(const std::string& message)
{
	return message.find("\r\n\r\n") != message.npos;
}

#include <iostream>
void	Server::processMessages()
{
	// TODO: get all ready clients instead of just the first one
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;

	if (client.socketFD == listenerSocketFD)
		return ;
	if (!client.requestLineFound && endOfRequestLineFound(client.message))
	{
		request.parseRequestLine(client.message);
		client.requestLineFound = true;
	}
	if (client.requestLineFound &&
		!client.headersFound &&
		endOfHeaderFound(client.message))
	{
		request.parseHeaders(client.message);
		client.headersFound = true;
	}
	if (client.requestLineFound && client.headersFound)
	{
		int	bodyLength = request.find<int>("Content-Length");
		if (bodyLength == std::numeric_limits<int>::min())
			bodyLength = 0;

		request.parseMessageBody(client.message);
		if (client.message.size() == (size_t)bodyLength)
		{
			readyRequests.push(request);
			logger.logRequest(*this, request);
		}
	}
}

void	Server::processReadyRequests()
{
	while (!readyRequests.empty())
	{
		Request&	request = readyRequests.front();
		Response	response = handleRequest(request);

		response.socketFD = request.socketFD;
		response.destAddress = request.srcAddress;
		readyResponses.push(response);
		readyRequests.pop();
	}
}

void	Server::generateResponses()
{
	while (!readyResponses.empty())
	{
		Response&	response = readyResponses.front();
		logger.logResponse(*this, response);

		std::string	formattedResponse = response.toString();
		send(response.socketFD, formattedResponse.c_str(), formattedResponse.size(), 0);
		
		// TODO: is this where we close the connection?
		close(response.socketFD);
		epoll_ctl(epollFD, EPOLL_CTL_DEL, response.socketFD, 0);
		clients.erase(clients.find(response.socketFD));
		readyResponses.pop();
	}
}

Response	Server::handleRequest(const Request& request) const
{
	Response	response;

	try
	{
		if (request.method == GET)
		{
			get(response, request);
		}
		else if (request.method == POST)
		{
			post(response, request);
		}
		else if (request.method == PUT)
		{
			put(response, request);
		}
		else if (request.method == DELETE)
		{
			delete_(response, request);
		}
		else if (request.method == HEAD)
		{
			head(response, request);
		}
	}
	catch (const Response& e)
	{
		response.httpVersion = e.httpVersion;
		response.statusCode = e.statusCode;
		response.reasonPhrase = e.reasonPhrase;
	}
	return response;
}
