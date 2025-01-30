/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:24:45 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/29 22:41:20 by kecheong         ###   ########.fr       */
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

// TODO: the receiving/processing here is all over the place and not well defined
Request	Server::receiveRequest(int fd) const
{
	char				buf[1024] = {};
	std::string			message;
	int					retval;

	while ((retval = recv(fd, buf, sizeof buf-1, 0)) > 0)
	{
		// TODO: better error handling
		if (retval == -1)
		{
			error("recv failed");
		}
		buf[retval] = '\0';
		message += buf;
		// TODO: what if the client sends only the request line,
		// now it breaks out of the loop because EWOULDBLOCK is returned from recv
		// but logically, what should happen after?
		if (message.find("\r\n\r\n") != message.npos)
			break ;
	}
	// HACK: this shouldn't be here, just a hack for now
	while (endOfHeaderNotFound(message) &&
		  (retval = recv(fd, buf, sizeof buf-1, 0) > 0))
	{
		buf[retval] = '\0';
		message += buf;
	}
	// if there is a Content-Length field, there is a message body
	std::string				messageBody;
	std::string::size_type	pos = message.find("Content-Length: ");
	size_t					bodyLength = 0;
	if (pos != message.npos)
	{
		std::string	numStr = message.substr(pos + sizeof "Content-Length:");
		bodyLength = std::atoi(numStr.c_str());
		messageBody = message.substr(message.find("\r\n\r\n")+4);
	}
	while (bodyLength > 0)
	{
		ssize_t	bytes = recv(fd, buf, sizeof buf-1, 0);
		if (bytes > 0)
		{
			buf[bytes] = '\0';
			messageBody += buf;
			bodyLength -= bytes;
		}
	}
	message += messageBody;

	// TODO: fix messageBody construction here
	Request	request = constructRequest(message);

	std::string::size_type	messageBodyStart = message.find("\r\n\r\n")+4;
	if (messageBodyStart != message.npos)
	{
		request.messageBody = message.substr(messageBodyStart);
	}

	// TODO: is this where we delete the fd? what if
	// the connection is kept alive
	epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, NULL);
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
	send(socketFD, message.c_str(), message.size(), 0);
}
