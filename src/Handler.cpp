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

// TODO: this belongs somewhere else
Request	constructRequest(const std::string &line);

Request	Server::receiveRequest(int fd) const
{
	char		buf[1024] = {};
	std::string	message;
	int			retval;

	while ((retval = recv(fd, buf, sizeof buf-1, 0)) > 0)
	{
		// TODO: better error handling
		if (retval == -1)
		{
			error("recv failed");
		}
		buf[retval] = '\0';
		message += buf;
		if (message.find("\r\n\r\n") != message.npos)
			break ;
	}

	Request	request = constructRequest(message);

	// TODO: writing and seeing this is pain
	std::map<std::string,std::string>::iterator it = request.headers.find("Content-Length");
	int	contentLength = 0;
	if (it != request.headers.end())
	{
		contentLength = std::atoi(it->second.c_str());
	}
	std::string	msgBody;
	while (contentLength > 0)
	{
		ssize_t	bytes = recv(fd, buf, sizeof buf-1, 0);
		if (bytes > 0)
		{
			buf[bytes] = '\0';
			msgBody += buf;
			contentLength -= bytes;
		}
		else break;
	}
	request.messageBody += msgBody;
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

void	Server::sendResponse(int socketFD, const Response& response) const
{
	std::stringstream	ss;
	std::string			message;

	ss << "HTTP/" << response.httpVersion
	   << " " << response.statusCode
	   << " " << response.reasonPhrase << "\r\n" << "\r\n\r\n"
	   << response.messageBody;

	message = ss.str();
	send(socketFD, message.c_str(), message.size(), 0);
}
