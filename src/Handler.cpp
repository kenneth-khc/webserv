/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:24:45 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/01 12:44:43 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <cstdlib>
#include "Server.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "ErrorCode.hpp"

#include "debugUtils.hpp"
#include <iostream>
#include <vector>

// TODO: this belongs somewhere else
Request	constructRequest(const std::string &line);


static bool	endOfHeaderNotFound(const std::string& message)
{
	return message.find("\r\n\r\n") == message.npos;
}

static bool	endOfRequestLineNotFound(const std::string& message)
{
	return message.find("\r\n") == message.npos;
}

ssize_t	readIntoBuffer(std::string& message, int socketFD)
{
	std::vector<char>	vect;
	const size_t		READ_SIZE = 1024;
	vect.resize(READ_SIZE);
	ssize_t	retval;

	retval = recv(socketFD, &vect[0], READ_SIZE, 0);
	if (retval > 0)
	{
		/*buf[retval] = '\0';*/
		for (size_t i = 0; i < vect.size(); ++i)
		{
			std::cout << vect[i];
			std::flush(std::cout);
			message.push_back(vect[i]);
		}
	}
	return retval;
}

ssize_t	Server::receiveBytes(Client& client)
{
	ssize_t	bytes = recv(client.socketFD,
						 &client.messageBuffer[0],
						 client.messageBuffer.size(),
						 0);
	//
	// TODO: fix where this belongs
	client.request.socketFD = client.socketFD;
	client.request.srcAddress = client.address;

	if (bytes > 0)
	{
		for (size_t i = 0; i < client.messageBuffer.size(); ++i)
		{
			client.message.push_back(client.messageBuffer[i]);
		}
	}
	return bytes;
}

// TODO: the receiving/processing here is all over the place and not well defined
Request	Server::receiveRequest(int fd) const
{
	Request		request;
	std::string	message;
	ssize_t		bytes;

	while (endOfRequestLineNotFound(message))
	{
		bytes = readIntoBuffer(message, fd);
		std::cout << "bytes: " << bytes << '\n';
		// TODO: errors
		if (bytes < 0)
		{
			throw Response(BadRequest400());
		}
	}
	request.parseRequestLine(message);
	while (endOfHeaderNotFound(message))
	{
		bytes = readIntoBuffer(message, fd);
		if (bytes < 0)
		{
			throw Response(BadRequest400());
		}
	}
	request.parseHeaders(message);

	// if there is a Content-Length field, there is a message body
	std::string	messageBody;
	int	bodyLength = request.find<int>("Content-Length");
	if (bodyLength > 0)
	{
		messageBody += message;
		bodyLength -= messageBody.size();
	}
	while (bodyLength > 0)
	{
		std::string	temp;
		bytes = readIntoBuffer(temp, fd);
		if (bytes < 0)
			break;
		if (bytes > 0)
		{
			messageBody += temp;
			bodyLength -= bytes;
		}
	}
	request.parseMessageBody(messageBody);
	// TODO: is this where we delete the fd? what if
	// the connection is kept alive
	
	/*epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, NULL);*/
	return request;
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
	catch (const ErrorCode& e)
	{
		response.httpVersion = e.httpVersion;
		response.statusCode = e.statusCode;
		response.reasonPhrase = e.reasonPhrase;
	}
	return response;
}

void	Server::sendResponse(int socketFD, Response& response) const
{
	std::string	message = response.toString();
	std::cout << "before send\n";
	send(socketFD, message.c_str(), message.size(), 0);
	std::cout << "after send\n";
}
