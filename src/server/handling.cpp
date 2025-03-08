/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 04:05:29 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/07 19:28:10 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <unistd.h>
#include "Optional.hpp"
#include "String.hpp"
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "Cookie.hpp"
#include "Base64.hpp"
#include "connection.hpp"
#include "date.hpp"
#include "contentLength.hpp"

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

void	Server::processMessages()
{
	if (listenerSocketFD == readyEvents[0].data.fd)
		return ;

	// TODO: get all ready clients instead of just the first one
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;

	try
	{
		if (!request.requestLineFound && client.endOfRequestLineFound())
		{
			request.parseRequestLine(client.message);
			request.requestLineFound = true;
		}
		if (request.requestLineFound &&
			!request.headersFound &&
			client.endOfHeaderFound())
		{
			request.parseHeaders(client.message);
			request.headersFound = true;

			Optional<String>	contentLength = request["Content-Length"];
			Optional<String>	transferEncoding = request["Transfer-Encoding"];

			if (contentLength.exists && transferEncoding.exists)
			{
				request.headers.erase(Request::stringToLower("Content-Length"));
			}
			else if (!contentLength.exists && !transferEncoding.exists)
			{
				request.insert(Request::stringToLower("Content-Length"), 0);
			}
			else if (contentLength.exists && !transferEncoding.exists)
			{
				if (isContentLengthHeader(contentLength.value) == false)
					throw BadRequest400();
			}
			else if (!contentLength.exists && transferEncoding.exists)
			{
				transferEncoding.value = Request::stringToLower(transferEncoding.value);
				if (transferEncoding.value.find("chunked").exists == false)
					throw BadRequest400();
			}
		}
		if (request.requestLineFound &&
			request.headersFound &&
			!request.messageBodyFound)
		{
			request.parseMessageBody(client.message);
		}
		if (request.messageBodyFound)
		{
			readyRequests.push(request);
			logger.logRequest(*this, request, client);
		}
	}
	catch (const ErrorCode &e)
	{
		Response	response;
		response.insert("Server", name);
		response = e;
		readyResponses.push(response);
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

Response	Server::handleRequest(Request& request)
{
	Response	response;

	response.insert("Server", name);
	request.parseCookieHeader();
	processCookies(request, response);
	try
	{
		if (request.method == "GET" || request.method == "HEAD")
		{
			get(response, request);
		}
		else if (request.method == "POST")
		{
			post(response, request);
		}
		else if (request.method == "DELETE")
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

void	Server::generateResponses()
{
	while (!readyResponses.empty())
	{
		Client&		client = clients[readyEvents[0].data.fd];
		Response&	response = readyResponses.front();
		std::string	formattedResponse = response.toString();

		send(client.socketFD, formattedResponse.c_str(), formattedResponse.size(), 0);
		logger.logResponse(*this, response, client);

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
