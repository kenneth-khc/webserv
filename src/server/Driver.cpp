/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/05 15:53:18 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "PathHandler.hpp"
#include "Server.hpp"
#include "Configuration.hpp"
#include "SetupError.hpp"
#include "contentLength.hpp"
#include "connection.hpp"
#include "ErrorCode.hpp"
#include "DoneState.hpp"
#include "KeepAliveTimer.hpp"
#include <deque>
#include <cstddef>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <algorithm>
#include <cstdio>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

Driver::Driver():
webServerName("42webserv"),
http(),
epollTimeout(-1),
epollFD(-1),
maxEvents(1021),
readyEvents(new epoll_event[maxEvents]),
numReadyEvents(0),
listeners(),
establishedSockets(),
clients()
{
}

void	Driver::initialize(const Configuration& config)
{
	Server::pathHandler.setPrefix(config.get("prefix").parameters[0]);

	const Directive&		httpBlock = config.get("http");
	std::vector<Directive*>	serverBlocks = httpBlock.getDirectives("server");

	this->http = httpBlock;
	for (size_t i = 0; i < serverBlocks.size(); ++i)
	{
		const Directive&	serverBlock = *serverBlocks[i];
		const Server&		httpServer = Server(serverBlock, listeners);
		http.addServer(httpServer);
	}

	/* Epoll configuration */
	epollFD = epoll_create(1);
	globalEpollFD = epollFD;
	globalCgis = &cgis;
	if (epollFD == -1)
	{
		std::perror("epoll_create() failed");
		std::exit(1);
	}
	for (std::map<int, Socket>::const_iterator it = listeners.begin();
		 it != listeners.end(); ++it)
	{
		addToEpoll(it->second.fd, EPOLLIN);
	}
}

Driver::Driver(const Configuration& config):
	webServerName("42webserv"),
	http(config.get("http")),
	epollTimeout(-1),
	epollFD(-1),
	maxEvents(1021),
	readyEvents(new epoll_event[maxEvents]),
	numReadyEvents(0),
	/* Sockets and Clients */
	listeners(),
	establishedSockets(),
	clients()
{
	Server::pathHandler.setPrefix(config.get("prefix").parameters[0]);
	std::vector<Directive*>	serverBlocks = config.get("http")
												 .getDirectives("server");
	for (size_t i = 0; i < serverBlocks.size(); ++i)
	{
		const Directive&	serverBlock = *serverBlocks[i];
		const Server&		httpServer = Server(serverBlock, listeners);
		http.addServer(httpServer);
	}

	/* Epoll configuration */
	epollFD = epoll_create(1);
	if (epollFD == -1)
	{
		std::perror("epoll_create() failed");
		std::exit(1);
	}
	for (std::map<int, Socket>::const_iterator it = listeners.begin();
		 it != listeners.end(); ++it)
	{
		addToEpoll(it->second.fd, EPOLLIN);
	}
}

Driver::~Driver()
{
	delete[] readyEvents;
	close(epollFD);
	for (std::map<int,Socket>::iterator	it = listeners.begin();
		 it != listeners.end(); ++it)
	{
		close(it->first);
	}
	for (std::map<int,Socket>::iterator	it = establishedSockets.begin();
		 it != establishedSockets.end(); ++it)
	{
		close(it->first);
	}
}

int	Driver::epollWait()
{
	if (epollTimeout != -1)
	{
		epollTimeout *= 1000;
	}

	numReadyEvents = epoll_wait(epollFD, readyEvents, maxEvents, epollTimeout);
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
	std::set<Timer*>	activeTimers;

	for (int i = 0; i < numReadyEvents; ++i)
	{
		currEvent = &readyEvents[i];
		const int&	fd = currEvent->data.fd;
		std::map<int,Socket>::iterator	it = listeners.find(fd);
		if (it != listeners.end())
		{
			const Socket&	listener = it->second;
			Socket			clientSocket = listener.accept();
			int				fd = clientSocket.fd;
			establishedSockets[fd] = clientSocket;
			addToEpoll(fd, EPOLL_EVENTS(EPOLLIN | EPOLLOUT));

			Client	newClient(&establishedSockets[fd], &listener);
			std::vector<Server>::iterator	serverIt =
			std::find_if(http.servers.begin(),
						 http.servers.end(),
						 isDefaultListener(&listener));
			newClient.server = &(*serverIt);

			clients[fd] = newClient;

			Logger::logConnection(Logger::ESTABLISHED, fd, newClient);
			activeTimers.insert(clients[fd].timer);
			continue ;
		}

		std::map<int, Client>::iterator	clientIt = clients.find(fd);
		std::map<int, CGI *>::iterator	cgiIt = cgis.find(fd);

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
			}
			if (currEvent->events & EPOLLOUT)
			{
				processRequest(clientIt, activeTimers);
				if (!clientIt->second.responseQueue.empty())
				{
					sendResponse(clientIt, activeTimers);
				}
			}
		}
		else if (cgiIt != cgis.end())
		{
			processCGI(cgiIt, activeTimers);
		}
	}

	for (std::set<Timer*>::iterator it = activeTimers.begin(); it != activeTimers.end(); it++)
	{
		(*it)->update();
	}
}

void	Driver::receiveMessage(std::map<int, Client>::iterator& clientIt)
{
	Client&	client = clientIt->second;

	client.receiveBytes();
}

// TODO: this should probably also check that the port matches
Optional<Server*>	Driver::matchServerName(const String& hostname)
{
	for (std::vector<Server>::iterator it = http.servers.begin();
		 it != http.servers.end();
		 ++it)
	{
		const std::vector<String>&	serverNames = it->domainNames;
		if (std::find(serverNames.begin(),
					  serverNames.end(), hostname) != serverNames.end())
		{
			Server*	server = &(*it);
			return makeOptional(server);
		}
	}
	return makeNone<Server*>();
}

void	Driver::processRequest(std::map<int, Client>::iterator& clientIt, std::set<Timer*>& activeTimers)
{
	Client&	client = clientIt->second;

	while (client.message.length() > 0) {
		Request&			request = client.requestQueue.back();
		String::size_type	initialMessageLength = client.message.length();

		try
		{
			if (client.responseQueue.size() != client.requestQueue.size())
			{
				client.responseQueue.push_back(Response());
				client.responseQueue.back().insert("Server", webServerName);
			}
			while (request.processState(client, logger)->getState() != RequestState::DONE)
			{
				if (initialMessageLength == client.message.length())
				{
					activeTimers.insert(client.timer);
					return ;
				}
				else
				{
					initialMessageLength = client.message.length();
				}
			}

			String				host = request.find< Optional<String> >("Host")
											  .value_or("");
			if (host.find(':'))
			{
				host = host.consumeUntil(":").value;
			}

			// this picks the configuration block to use depending on server_name
			// or defaulting back to first server with the matching port
			Server*		server = matchServerName(host)
								.value_or(client.server);

			server->handleRequest(*this, client, request, client.responseQueue.back());
		}
		catch (const ErrorCode &e)
		{
			delete request.state;
			request.state = new DoneState();
			request.processState(client, logger);
			client.responseQueue.back() = e;
		}

		client.requestQueue.push_back(Request());
		if (client.responseQueue.back()["Connection"].value == "close")
		{
			return ;
		}
	}
}

void	Driver::processCGI(std::map<int, CGI*>::iterator& cgiIt, std::set<Timer*>& activeTimers)
{
	CGI&	cgi = *(cgiIt->second);
	Client&	client = cgi.client;

	try {
		if (currEvent->events & EPOLLIN)
		{
			cgi.output->fetch(activeTimers);
		}
		if (currEvent->events & EPOLLOUT)
		{
			cgi.input->feed(activeTimers);
		}
		if (currEvent->events & EPOLLHUP)
		{
			int	stat_loc = 0;

			if (waitpid(cgi.pid, &stat_loc, WNOHANG) == cgi.pid) {
				if (stat_loc != 0)
					throw InternalServerError500();
			}
			cgi.output->fetch(activeTimers);
			if (cgi.response.processStage & Response::DONE)
			{
				activeTimers.erase(cgi.timer);
				client.cgis.erase(std::find(client.cgis.begin(), client.cgis.end(), &cgi));
				delete &cgi;
				return ;
			}
		}
	}
	catch (const ErrorCode &e) {
		cgi.response = e;
		cgi.input->close();
		cgi.output->close();
		activeTimers.erase(cgi.timer);
		client.cgis.erase(std::find(client.cgis.begin(), client.cgis.end(), &cgi));
		delete &cgi;
	}
}

void	Driver::sendResponse(std::map<int, Client>::iterator& clientIt, std::set<Timer*>& activeTimers)
{
	Client&		client = clientIt->second;
	Request&	request = client.requestQueue.front();
	Response&	response = client.responseQueue.front();

	if (!(response.processStage & Response::SEND_READY) && response.isReady() == false)
	{
		return ;
	}

	if (request.method != "HEAD")
	{
		response.appendMessageBody();
	}
	client.sendBytes(response.formatted);

	if (response.processStage & Response::DONE)
	{
		Logger::logResponse(response, client);

		if (response.closeConnection == true)
		{
			closeConnection(clientIt, Logger::CLOSE);
		}
		else
		{
			client.requestQueue.pop_front();
			client.responseQueue.pop_front();
			if (client.message.length() == 0 && client.requestQueue.front().state == 0)
			{
				if (client.timer == 0 || client.timer->getType() != Timer::KEEP_ALIVE)
				{
					delete client.timer;
					client.timer = new KeepAliveTimer();
				}
				activeTimers.insert(client.timer);
			}
		}
	}
}

void	Driver::updateEpollTimeout()
{
	std::priority_queue<std::time_t>	timeoutPriority;

	std::map<int, Client>::const_iterator clientIt = clients.begin();
	while (clientIt != clients.end())
	{
		const Client	&client = clientIt->second;

		if (client.timer != 0)
		{
			timeoutPriority.push(client.timer->getTimeoutTime());
		}

		std::vector<CGI*>::const_iterator cgiIt = client.cgis.begin();
		while (cgiIt != client.cgis.end())
		{
			const CGI	&cgi = *(*cgiIt);

			if (cgi.timer != 0)
			{
				timeoutPriority.push(cgi.timer->getTimeoutTime());
			}
			cgiIt++;
		}
		clientIt++;
	}

	if (timeoutPriority.size() == 0)
		epollTimeout = -1;
	else
		epollTimeout = timeoutPriority.top() - Time::getTimeSinceEpoch();
}

void	Driver::closeConnection(std::map<int, Client>::iterator clientIt, int logFlag)
{
	const int&	fd = clientIt->first;
	Client&		client = clientIt->second;

	Logger::logConnection(logFlag, fd, client);
	epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, 0);
	close(fd);
	establishedSockets.erase(establishedSockets.find(fd));

	std::vector<CGI *>::iterator cgiIt = client.cgis.begin();
	while (cgiIt != client.cgis.end())
	{
		kill((*cgiIt)->pid, SIGKILL);
		delete *cgiIt;
		cgiIt++;
	}

	clients.erase(clientIt);
}

void	Driver::monitorTimers()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		Client	&client = it->second;

		if (client.timer != 0 && client.timer->isTimeout() == true)
		{
			closeConnection(it++, client.timer->getLogType());
		}
		else
		{
			std::vector<CGI*>::iterator cgiIt = client.cgis.begin();
			while (cgiIt != client.cgis.end())
			{
				if ((*cgiIt)->timer->isTimeout() == true)
				{
					(*cgiIt)->response = InternalServerError500();
					delete *cgiIt;
					cgiIt = client.cgis.erase(cgiIt);
				}
				else
				{
					cgiIt++;
				}
			}
			it++;
		}
	}
}

void	Driver::addToEpoll(int fd, EPOLL_EVENTS events)
{
	epoll_event	ep = epoll_event();
	ep.events = events;
	ep.data.fd = fd;

	int	retval = epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &ep);
	if (retval == -1)
	{
		std::perror("epoll_ctl(ADD) failed");
		std::exit(1);
	}
}
