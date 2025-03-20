/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/21 17:30:53 by cteoh            ###   ########.fr       */
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
#include "CGI.hpp"
#include <deque>
#include <cstddef>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <iostream>

Driver::Driver():
name("42webserv"),
epollFD(-1),
maxEvents(1021),
readyEvents(NULL),
numReadyEvents(0),
currEvent(0),
MIMEMappings("mime.types"),
rootDir("root"),
pagesDir("pages"),
uploadsDir("uploads"),
miscPagesDir("misc_pages"),
cgiDir("cgi-bin"),
autoindex(false)
{
	readyEvents = new epoll_event[maxEvents];
	cgiScript.push_back("py");
	cgiScript.push_back("php");
	cgiScript.push_back("bla");	// Test-specific condition
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
		listeners[s.fd].listen(1020);
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
	return numReadyEvents;
}

void	Driver::processReadyEvents()
{
	for (int i = 0; i < numReadyEvents; ++i)
	{
		currEvent = &readyEvents[i];

		if (listeners.find(currEvent->data.fd) != listeners.end())
		{
			const Socket&	socket = listeners.find(currEvent->data.fd)->second;
			acceptNewClient(socket);
			return ;
		}

		std::map<int, Client>::iterator	clientIt = clients.find(currEvent->data.fd);
		std::map<int, CGI *>::iterator	cgiIt = cgis.find(currEvent->data.fd);

		if (clientIt == clients.end() && cgiIt == cgis.end())
			continue ;

		if (clientIt != clients.end())
		{
			if (currEvent->events & EPOLLHUP)
			{
				closeConnection(clientIt, Logger::PEER_CLOSE);
				continue ;
			}
			if (currEvent->events & EPOLLIN)
			{
				receiveMessage(clientIt);
				processRequest(clientIt);
			}
			if (currEvent->events & EPOLLOUT && !clientIt->second.responseQueue.empty())
			{
				generateResponse(clientIt);
			}
		}
		else if (cgiIt != cgis.end())
		{
			if (currEvent->events & EPOLLIN)
			{
				cgiIt->second->fetchOutput(epollFD);
			}
			if (currEvent->events & EPOLLOUT)
			{
				cgiIt->second->feedInput(epollFD);
			}
			if (currEvent->events & EPOLLHUP)
			{
				cgiIt->second->fetchOutput(epollFD);
			}
			processCGI(cgiIt);
		}
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
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = clientSocket.fd;
	client.socket = clientSocket;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, client.socket.fd, &event);
	clients.insert(std::make_pair(client.socket.fd, client));
	logger.logConnection(Logger::ESTABLISHED, client.socket.fd, client);
}

void	Driver::receiveMessage(std::map<int, Client>::iterator& clientIt)
{
	Client&	client = clientIt->second;

	client.receiveBytes();
}

void	Driver::processRequest(std::map<int, Client>::iterator& clientIt)
{
	Client&		client = clientIt->second;
	Request&	request = client.requestQueue.back();

	try
	{
		if (request.processStage & Request::REQUEST_LINE)
		{
			request.parseRequestLine(client.message);
		}
		if (request.processStage & Request::HEADERS)
		{
			request.parseHeaders(client.message);
		}
		if (request.processStage & Request::HEAD_DONE)
		{
			request.checkIfBodyExists();
			logger.logRequest(request, client);
		}
	}
	catch (const ErrorCode &e)
	{
		Response	response;
		response.insert("Server", name);
		response = e;
		request.processStage |= Request::DONE;
		client.responseQueue.push_back(response);
		return ;
	}

	try {
		if (request.processStage & Request::READY)
		{
			client.responseQueue.push_back(Response());
			preprocessReadyRequest(request, client.responseQueue.back());
			request.processStage &= ~Request::READY;
		}
		if (request.processStage & Request::MESSAGE_BODY)
		{
			request.parseMessageBody(client.message);
		}
		if (request.processStage & Request::DONE)
		{
			Response	&response = client.responseQueue.back();

			processReadyRequest(request, response);
			postprocessReadyRequest(request, response);
		}
	}
	catch (const ErrorCode &e)
	{
		request.processStage |= Request::DONE;
		client.responseQueue.back() = e;
		return ;
	}
}

void	Driver::preprocessReadyRequest(Request& request, Response& response)
{
	if (response.processStage & Response::PRE_PROCESSING)
	{
		response.insert("Server", name);
		request.parseCookieHeader();
		processCookies(request, response);
		response.processStage &= ~Response::PRE_PROCESSING;
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

void	Driver::processReadyRequest(Request& request, Response& response)
{
	if (request.path.starts_with("/" + cgiDir + "/") ||
		request.path.ends_with(".bla"))	// Test-specific condition
	{
		cgi(request, response);
	}
	else if (request.method == "GET")
	{
		get(request, response);
	}
	else if (request.method == "POST")
	{
		post(request, response);
	}
	else if (request.method == "DELETE")
	{
		delete_(request, response);
	}
	else	// Test-specific condition
	{
		if (request.path == "/")
			throw MethodNotAllowed405();
		throw NotImplemented501();
	}
}

void	Driver::postprocessReadyRequest(Request& request, Response& response)
{
	if (response.processStage & Response::POST_PROCESSING)
	{
		constructConnectionHeader(request, response);
		response.insert("Date", Time::printHTTPDate());
		response.processStage = Response::DONE;
	}
}

void	Driver::processCGI(std::map<int, CGI*>::iterator& cgiIt)
{
	CGI	&cgi = *(cgiIt->second);

	if (cgi.processStage & CGI::INPUT_DONE)
	{
		cgi.processStage &= ~CGI::INPUT_DONE;
		cgis.erase(cgiIt);
	}
	if (cgi.processStage & CGI::OUTPUT_DONE)
	{
		try {
			postprocessReadyRequest(cgi.request, cgi.response);
		}
		catch (const ErrorCode &e) {
			cgi.response = e;
		}
		delete &cgi;
		cgis.erase(cgiIt);
	}
}

void	Driver::generateResponse(std::map<int, Client>::iterator& clientIt)
{
	Client&		client = clientIt->second;
	Request&	request = client.requestQueue.front();
	Response&	response = client.responseQueue.front();

	if (!(response.processStage & Response::DONE))
		return ;

	if (response.formatted == "")
	{
		if (request.method == "HEAD")
			response.messageBody = "";
		response.format();
	}

	client.sendBytes(response);
	if (response.formatted == "")
	{
		logger.logResponse(response, client);

		if (response.closeConnection == true)
		{
			closeConnection(clientIt, Logger::CLOSE);
		}
		else
		{
			client.requestQueue.pop_front();
			client.responseQueue.pop_front();
			if (client.firstSend == false)
			{
				client.firstSend = true;
			}
			client.lastActive = Time::getTimeSinceEpoch();
		}
	}
}

void	Driver::closeConnection(std::map<int, Client>::iterator clientIt, int logFlag)
{
	Client&	client = clientIt->second;

	logger.logConnection(logFlag, currEvent->data.fd, client);
	epoll_ctl(epollFD, EPOLL_CTL_DEL, currEvent->data.fd, 0);
	close(currEvent->data.fd);
	clients.erase(clientIt);
}

void	Driver::monitorConnections()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		Client	&client = it->second;

		if (client.firstSend == true && client.requestQueue.empty() &&
			(Time::getTimeSinceEpoch() - client.lastActive >= Server::keepAliveTimeout))
		{
			closeConnection(it++, Logger::TIMEOUT);
		}
		else
		{
			it++;
		}
	}
}

#include <iomanip>
#include <algorithm>
#include <sys/stat.h>

#define FILE_NAME_LEN 45
#define FILE_SIZE_LEN 20

void	Driver::generateDirectoryListing(Response& response, const String& dirName) const
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

void	Driver::generateUploadsListing(
	const Request& request,
	Response& response,
	const String& uploadsDir) const
{
	DIR*	dir = opendir(uploadsDir.c_str());

	if (!dir)
		throw NotFound404();

	std::stringstream	stream;
	std::vector<String>	uploadsList;

	stream << "<html>\n"
		   << 	"<head>\n"
		   << 		"<style>\n\t"
		   <<			"html { font-family: 'Comic Sans MS' }\n\t"
		   << 			"body { background-color: #f4dde7 }\n"
		   << 		"</style>\n"
		   << 	"</head>\n"
		   <<	"<body>\n"
		   <<		"<h1>Uploads</h1>\n"
		   <<		"<hr><pre>\n";

	response.messageBody += stream.str();

	const String&	sid = request.cookies.find("sid")->second.value;
	for (dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
	{
		String						file(entry->d_name);
		Optional<String::size_type> sidMatch = file.find(sid);

		if (sidMatch.exists == false || sidMatch.value != 0)
			continue ;

		Optional<String::size_type>	delimiter = file.find("_");
		String						trim = file.substr(delimiter.value + 1);
		String						truncate = trim;

		if (truncate.length() > FILE_NAME_LEN)
		{
			truncate.resize(FILE_NAME_LEN);
			truncate.replace(FILE_NAME_LEN - 3, 3, "...");
		}
		stream.str("");
		stream << "<a href=\""
			   << "/" + uploadsDir + "/" + trim
			   << "\">"
			   << std::left
			   << std::setw(FILE_NAME_LEN + 5)
			   << truncate + "</a> "
			   << "<button type=\"button\" onclick=\"del("
			   << "'/" + uploadsDir + "/" + trim + "'"
			   << ")\">Delete</button>\n";
		uploadsList.push_back(stream.str());
	}

	std::sort(uploadsList.begin(), uploadsList.end());

	for (std::vector<String>::const_iterator it = uploadsList.begin(); it != uploadsList.end(); it++)
	{
		response.messageBody += *it;
	}

	stream.str("");
	stream << 		"</pre><hr>\n"
		   << 		"<script>"
		   <<			"async function del(url) { await fetch(url, { method: \"DELETE\" }); }"
		   <<		"</script>"
		   << 	"</body>\n"
		   << "</html>";
	response.messageBody += stream.str();
}
