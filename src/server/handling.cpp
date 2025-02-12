/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 04:05:29 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 15:55:17 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "connection.hpp"
#include "date.hpp"

ssize_t	Server::receiveBytes(Client& client)
{
	ssize_t	bytes = recv(client.socketFD,
						 &client.messageBuffer[0],
						 client.messageBuffer.size(), 0);

	if (bytes > 0)
	{
		for (ssize_t i = 0; i < bytes; ++i)
		{
			client.message += client.messageBuffer[i];
		}
		if (client.firstDataRecv == false)
		{
			client.firstDataRecv = true;
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
void	Server::processMessages()
{
	if (listenerSocketFD == readyEvents[0].data.fd)
		return ;

	// TODO: get all ready clients instead of just the first one
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;

	if (!request.requestLineFound && endOfRequestLineFound(client.message))
	{
		request.parseRequestLine(client.message);
		request.requestLineFound = true;
	}
	if (request.requestLineFound &&
		!request.headersFound &&
		endOfHeaderFound(client.message))
	{
		request.parseHeaders(client.message);
		request.headersFound = true;
	}
	if (request.requestLineFound && request.headersFound)
	{
		Optional<int>	bodyLength = request.find< Optional<int> >("Content-Length");

		request.messageBody = client.message;
		if (client.message.size() == (size_t)bodyLength.value)
		{
			readyRequests.push(request);
			logger.logRequest(*this, request, &client.address);
		}
	}
}

void	Server::processReadyRequests()
{
	while (!readyRequests.empty())
	{
		Request&	request = readyRequests.front();
		Response	response = handleRequest(request);

		readyResponses.push(response);
		readyRequests.pop();
	}
}

void	Server::generateResponses()
{
	while (!readyResponses.empty())
	{
		Client&		client = clients[readyEvents[0].data.fd];
		Response&	response = readyResponses.front();
		logger.logResponse(*this, response);

		std::string	formattedResponse = response.toString();
		send(client.socketFD, formattedResponse.c_str(), formattedResponse.size(), 0);

		if (response.flags & Response::CONNECTION_CLOSE)
		{
			close(response.socketFD);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, client.socketFD, 0);
			clients.erase(clients.find(client.socketFD));
		}
		else
		{
			client.request = Request();
		}
		readyResponses.pop();
	}
}

Response	Server::handleRequest(const Request& request) const
{
	Response	response;

	/*const_cast<Request&>(request).method = POST;*/
	try
	{
		if (request.method == Request::GET)
		{
			get(response, request);
		}
		else if (request.method == Request::POST)
		{
			post(response, request);
		}
		else if (request.method == Request::PUT)
		{
			put(response, request);
		}
		else if (request.method == Request::DELETE)
		{
			delete_(response, request);
		}
		else if (request.method == Request::HEAD)
		{
			head(response, request);
		}
	}
	catch (const ErrorCode& e)
	{
		response = e;
	}
	constructConnectionHeader(request, response);
	response.insert("Date", Time::printHTTPDate());
	return response;
}
