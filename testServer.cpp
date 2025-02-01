/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/01 12:41:00 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include "debugUtils.hpp"

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
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;

	if (!client.requestLineFound && endOfRequestLineFound(client.message))
	{
		request.parseRequestLine(client.message);
		client.requestLineFound = true;
	}
	if (client.requestLineFound && endOfHeaderFound(client.message))
	{
		request.parseHeaders(client.message);
		client.headersFound = true;
	}
	if (client.requestLineFound && client.headersFound)
	{
		readyRequests.push(request);
		logger.logRequest(*this, request);
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

int	main()
{
	Server	server;

	server.startListening();
	server.initEpoll();

	std::cout << "Server is running...\n";
	while (1)
	{
		server.epollWait();
		server.processReadyEvents();
		try
		{
			server.processMessages();
			server.processReadyRequests();
			server.generateResponses();
		}
		catch (const Response& e)
		{
		}
	}
}
