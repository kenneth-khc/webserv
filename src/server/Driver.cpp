/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/11 15:54:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "Server.hpp"
#include "Configuration.hpp"
#include "ConfigErrors.hpp"
#include "contentLength.hpp"
#include "Directive.hpp"
#include "Utils.hpp"
#include "connection.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "Base64.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <iostream>

const unsigned int	Driver::timeoutValue = 5;

Driver::Driver():
name("42webserv"),
epollFD(-1),
maxEvents(1),
readyEvents(NULL),
numReadyEvents(0),
MIMEMappings("mime.types"),
rootDir("root"),
pagesDir("pages"),
uploadsDir("uploads"),
miscPagesDir("misc_pages"),
cgiDir("cgi-bin"),
autoindex(true)
{
	readyEvents = new epoll_event[maxEvents];
	cgiScript.push_back("py");
	cgiScript.push_back("php");
}

// TODO: refactor this crap holy
void	Driver::configureFrom(const Configuration& config)
{
	epollFD = epoll_create(1);
	if (epollFD == -1){
		// TODO: error handling
	}

	const Directive&		http = config.get("http");
	std::vector<Directive>	serverDirectives = http.getDirectives("server");
	for (size_t i = 0; i < serverDirectives.size(); ++i)
	{
		const Directive&	serverDirective = serverDirectives[i];
		try
		{
			configNewServer(serverDirective);
		}
		catch (const ConfigError& e)
		{
			std::cout << e.what() << '\n';
		}
	}
	epoll_event event = {};
	event.events |= EPOLLIN;

	for (std::map<int, Socket>::const_iterator it = listeners.begin();
		 it != listeners.end(); ++it)
	{
		event.data.fd = it->second.fd;
		int	retval = epoll_ctl(epollFD, EPOLL_CTL_ADD, event.data.fd, &event);
		if (retval == -1)
		{ //TODO: error handling
		}
	}
}

void	Driver::configNewServer(const Directive& directive)
{
	String	listenTo = directive.getParams<String>("listen").value_or("8000");
	int		portNum = to<int>(listenTo);
	Socket*	socket = NULL;
	if (listeners.find(portNum) == listeners.end())
	{
		Socket	s = Socket(portNum);
		listeners[s.fd] = s;
		listeners[s.fd].bind();
		listeners[s.fd].listen(1);
		// WARN: something sussy here with the socket pointer
		// what happens if I have a listen in another server
		// trying to point to an existing socket
		socket = &listeners[s.fd];
	}
	std::vector<String>	domainNames = directive.getParams< std::vector<String> >("server_name")
									  .value_or(std::vector<String>());
	Server	newServer(domainNames, portNum, socket);
	newServer.root = directive.getParams<String>("root").value_or("html");
	servers.push_back(newServer);
}

int	Driver::epollWait()
{
	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, 1000);
	// std::cout << "epoll_wait() returned with " << numReadyEvents
			//   << " ready event" << (numReadyEvents > 1 ? "s\n" : "\n");

	if (numReadyEvents == -1)
	{
		//TODO: error handling
	}
	else if (numReadyEvents > 0 && (readyEvents[0].events & EPOLLRDHUP))
	{
		//	Closes socket in cases where client-side closes the connection on their end.
		logger.logConnection(Logger::CLOSE, readyEvents[0].data.fd,
			clients[readyEvents[0].data.fd]);
		close(readyEvents[0].data.fd);
		epoll_ctl(epollFD, EPOLL_CTL_DEL, readyEvents[0].data.fd, 0);
		clients.erase(clients.find(readyEvents[0].data.fd));
		numReadyEvents = 0;
	}
	return numReadyEvents;
}

void	Driver::processReadyEvents()
{
	for (int i = 0; i < numReadyEvents; ++i)
	{
		const epoll_event&	ev = readyEvents[i];

		if (listeners.find(ev.data.fd) != listeners.end())
		{
			const Socket&	socket = listeners.find(ev.data.fd)->second;
			acceptNewClient(socket);
		}
		else if (ev.events & EPOLLIN)
		{
			Client&	client = clients[ev.data.fd];
			client.updateLastActive();
			receiveBytes(client);
		}
	}
}

void	Driver::processMessages()
{
	// TODO: get all ready clients instead of just the first one
	Client&		client = clients[readyEvents[0].data.fd];
	Request&	request = client.request;

	try
	{
		if (!request.requestLineFound && client.endOfRequestLineFound())
		{
			request.parseRequestLine(client.message);
		}
		if (request.requestLineFound &&
			!request.headersFound &&
			client.endOfHeaderFound())
		{
			request.parseHeaders(client.message);

			Optional<String>	contentLength = request["Content-Length"];
			Optional<String>	transferEncoding = request["Transfer-Encoding"];

			if (transferEncoding.exists)
			{
				if (contentLength.exists)
				{
					request.headers.erase(Request::stringToLower("Content-Length"));
				}
				transferEncoding.value = Request::stringToLower(transferEncoding.value);
				if (transferEncoding.value.find("chunked").exists == false)
				{
					throw BadRequest400();
				}
			}
			else if (contentLength.exists)
			{
				if (isContentLengthHeader(contentLength.value) == false)
				{
					throw BadRequest400();
				}
			}
			else
				request.ready = true;
		}
		if (request.requestLineFound &&
			request.headersFound &&
			!request.ready)
		{
			request.parseMessageBody(client.message);
		}
		if (request.ready)
		{
			readyRequests.push(request);
			logger.logRequest(request, client);
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

void	Driver::processReadyRequests()
{
	while (!readyRequests.empty())
	{
		Request&	request = readyRequests.front();
		Response	response = handleRequest(request);

		readyResponses.push(response);
		readyRequests.pop();
	}
}

void	Driver::processCookies(Request& request, Response& response)
{
	std::map<String, Cookie>&	cookies = request.cookies;
	Cookie						cookie;

	if (cookies.find("sid") == cookies.end())
	{
		String	sid = Base64::encode(Time::printHTTPDate());

		cookie = Cookie("sid", sid);
		cookie.path = Optional<String>("/");
		cookies.insert(std::make_pair("sid", cookie));
		response.insert("Set-Cookie", cookie.constructSetCookieHeader());
	}
	if (cookies.find("lang") == cookies.end())
	{
		cookie = Cookie("lang", "en");
		cookie.path = Optional<String>("/");
		cookies.insert(std::make_pair("lang", cookie));
		response.insert("Set-Cookie", cookie.constructSetCookieHeader());
	}
}

Response	Driver::handleRequest(Request& request)
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

void	Driver::generateResponses()
{
	while (!readyResponses.empty())
	{
		Client&		client = clients[readyEvents[0].data.fd];
		Response&	response = readyResponses.front();
		std::string	formattedResponse = response.toString();

		send(client.socket.fd, formattedResponse.c_str(), formattedResponse.size(), 0);
		logger.logResponse(response, client);

		if (response.flags & Response::CONNECTION_CLOSE)
		{
			close(client.socket.fd);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, client.socket.fd, 0);
			clients.erase(clients.find(client.socket.fd));
		}
		else
		{
			client.request = Request();
		}
		readyResponses.pop();
	}
}

void	Driver::monitorConnections()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		if (it->second.firstDataRecv == true && it->second.isTimeout() == true)
		{
			logger.logConnection(Logger::TIMEOUT, it->first, it->second);
			close(it->first);
			epoll_ctl(epollFD, EPOLL_CTL_DEL, it->first, 0);
			clients.erase(it++);
		}
		else
			it++;
	}
}

void	Driver::acceptNewClient(const Socket& socket)
{
	Client	client;

	client.addressLen = static_cast<socklen_t>(sizeof client.address);
	Socket	clientSocket;
	clientSocket.fd = accept(socket.fd, (sockaddr*)&client.address,
							 &client.addressLen);
	std::cout << ">> " << client.socket.fd << '\n';
	fcntl(client.socket.fd, F_SETFL, O_NONBLOCK);
	//++numClients;

	//	SO_LINGER prevents close() from returning when there's still data in
	//	the socket buffer. This avoids the "TCP reset problem" and allows
	//	graceful closure.
	linger	linger = {.l_onoff = 1, .l_linger = 5};
	setsockopt(client.socket.fd, SOL_SOCKET, SO_LINGER, &linger, sizeof linger);

	epoll_event	event = epoll_event();
	event.events = EPOLLIN | EPOLLRDHUP;
	event.data.fd = clientSocket.fd;
	client.socket = clientSocket;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, client.socket.fd, &event);
	clients.insert(std::make_pair(client.socket.fd, client));
	logger.logConnection(Logger::ESTABLISHED, client.socket.fd, client);
}

ssize_t	Driver::receiveBytes(Client& client)
{
	ssize_t	bytes = recv(client.socket.fd,
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

#include <iomanip>
#include <algorithm>
#include <sys/stat.h>

#define FILE_NAME_LEN 45
#define FILE_SIZE_LEN 20

void	Driver::generateDirectoryListing(Response& response, const std::string& dirName) const
{
	DIR*	dir = opendir(dirName.c_str());

	if (!dir)
		throw NotFound404();

	struct stat			statbuf;
	std::string			path = dirName;
	std::string			trimmedRootPath;
	std::stringstream	stream;

	if (path[path.length() - 1] != '/')
		path += "/";

	trimmedRootPath = path.substr(path.find_first_of('/'));
	stream << "<html>\n"
		   << 	"<head>\n"
		   << 		"<title>Index of " + trimmedRootPath + "</title>\n"
		   << 		"<style>\n\t"
		   << 			"html { font-family: 'Comic Sans MS' }\n\t"
		   << 			"body { background-color: #f4dde7 }\n\t"
		   << 			"p { display: inline }\n"
		   << 		"</style>\n"
		   << 	"</head>\n"
		   << 	"<body>\n"
		   << 		"<h1>Index of " + trimmedRootPath + "</h1>\n"
		   << 		"<hr><pre>\n";

	response.messageBody += stream.str();

	std::string					parentDir;
	std::vector<std::string>	directories;
	std::vector<std::string>	regularFiles;

	for (dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
	{
		std::string	d_name(entry->d_name);
		std::string	str;
		std::string	truncate;

		if (d_name == ".")
		{
			continue ;
		}

		stream.str("");
		// File/Directory Name
		truncate = (entry->d_type == DT_DIR) ? d_name + "/" : d_name;
		if (truncate.length() > FILE_NAME_LEN)
		{
			truncate.resize(FILE_NAME_LEN);
			if (entry->d_type == DT_DIR)
			{
				truncate.replace(FILE_NAME_LEN - 4, 4, ".../");
			}
			else
			{
				truncate.replace(FILE_NAME_LEN - 3, 3, "...");
			}
		}

		stream << "<a href=\""
			   << ((entry->d_type == DT_DIR) ? trimmedRootPath + d_name + "/" : trimmedRootPath + d_name)
			   << "\">"
			   << std::left
			   << std::setw(FILE_NAME_LEN + 5)
			   << truncate + "</a> ";

		str += stream.str();
		if (d_name == "..")
		{
			parentDir = str + "\n";
			continue ;
		}

		// Last Modified Date and File Size in Bytes
		std::stringstream	streamTwo;

		stream.str("");
		stat(path.c_str(), &statbuf);
		stream << "<p>";
		if (entry->d_type == DT_DIR)
		{
			streamTwo << "-";
		}
		else
		{
			streamTwo << statbuf.st_size;
		}
		stream << Time::printAutoindexDate(statbuf.st_mtim)
			   << " "
			   << std::right
			   << std::setw(FILE_SIZE_LEN)
			   << streamTwo.str()
			   << "</p>\n";

		str += stream.str();
		if (entry->d_type == DT_DIR)
			directories.push_back(str);
		else
			regularFiles.push_back(str);
	}

	std::sort(directories.begin(), directories.end());
	std::sort(regularFiles.begin(), regularFiles.end());

	response.messageBody += parentDir;
	for (std::vector<std::string>::const_iterator it = directories.begin(); it != directories.end(); it++)
	{
		response.messageBody += *it;
	}
	for (std::vector<std::string>::const_iterator it = regularFiles.begin(); it != regularFiles.end(); it++)
	{
		response.messageBody += *it;
	}

	stream.str("");
	stream << 		"</pre><hr>\n"
		   << 	"</body>\n"
		   << "</html>";
	response.messageBody += stream.str();
	closedir(dir);
}
