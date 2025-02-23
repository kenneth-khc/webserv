/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 04:05:29 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/24 03:05:24 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <unistd.h>
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "Cookie.hpp"
#include "Base64.hpp"
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

	try
	{
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
				client.message = "";
				logger.logRequest(*this, request, (sockaddr*)&client.address);
			}
		}
	}
	catch (const Response &e)
	{
		readyResponses.push(e);
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

		std::string	formattedResponse = response.toString();
		send(client.socketFD, formattedResponse.c_str(), formattedResponse.size(), 0);

		if (response.flags & Response::CONNECTION_CLOSE)
		{
			close(client.socketFD);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, client.socketFD, 0);
			clients.erase(clients.find(client.socketFD));
		}
		else
		{
			client.request = Request();
		}
		logger.logResponse(*this, response, (sockaddr*)&client.address);
		readyResponses.pop();
	}
}

void	Server::processCookies(Request& request, Response& response)
{
	std::map<String, Cookie>&	cookies = request.cookies;

	if (cookies.find("sid") == cookies.end())
	{
		String	sid = Base64::encode(Time::printHTTPDate());
		cookies.insert(std::make_pair("sid", Cookie("sid", sid)));
		response.insert("Set-Cookie", "sid=" + sid);
	}
	if (cookies.find("lang") == cookies.end())
	{
		cookies.insert(std::make_pair("lang", Cookie("lang", "en")));
		response.insert("Set-Cookie", "lang=en");
	}
}

Response	Server::handleRequest(Request& request)
{
	Response	response;

	request.parseCookieHeader();
	processCookies(request, response);
	try
	{
		if (request.method == Request::GET || request.method == Request::HEAD)
		{
			get(response, request);
		}
		else if (request.method == Request::POST)
		{
			post(response, request);
		}
		else if (request.method == Request::DELETE)
		{
			delete_(response, request);
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
