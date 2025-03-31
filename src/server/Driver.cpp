/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/01 01:25:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "Server.hpp"
#include "Configuration.hpp"
#include "ConfigErrors.hpp"
#include "Directive.hpp"
#include "Utils.hpp"
#include "connection.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "Base64.hpp"
#include "CGI.hpp"
#include "Request.hpp"
#include "DoneState.hpp"
#include "KeepAliveState.hpp"
#include <queue>
#include <deque>
#include <cstddef>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <algorithm>
#include <cstdio>

int					globalEpollFD;
std::map<int,CGI*>*	globalCgis;

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
	std::set<Client *>	activeClients;

	for (int i = 0; i < numReadyEvents; ++i)
	{
		currEvent = &readyEvents[i];
		std::map<int,Socket>::iterator	it = listeners.find(currEvent->data.fd);
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

			logger.logConnection(Logger::ESTABLISHED, fd, newClient);
			activeClients.insert(&clients[fd]);
			continue ;
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
			}
			if (currEvent->events & EPOLLOUT)
			{
				processRequest(clientIt, activeClients);
				if (!clientIt->second.responseQueue.empty())
				{
					generateResponse(clientIt, activeClients);
				}
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

	for (std::set<Client *>::iterator it = activeClients.begin(); it != activeClients.end(); it++)
	{
		(*it)->timer->update(*(*it)->server);
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

void	Driver::processRequest(std::map<int, Client>::iterator& clientIt, std::set<Client *>& activeClients)
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
					activeClients.insert(&client);
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

			request.client = &client;
			// this picks the configuration block to use depending on server_name
			// or defaulting back to first server with the matching port
			Server*		server = matchServerName(host)
								.value_or(request.client->server);

			server->handleRequest(request, client.responseQueue.back());
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

void	Driver::processCGI(std::map<int, CGI*>::iterator& cgiIt)
{
	CGI	&cgi = *(cgiIt->second);

	if (cgi.processStage & CGI::INPUT_DONE)
	{
		cgis.erase(cgiIt);
		cgi.processStage &= ~CGI::INPUT_DONE;
	}
	else if (cgi.processStage & CGI::OUTPUT_DONE)
	{
		try {
			constructConnectionHeader(cgi.request, cgi.response);
			cgi.response.insert("Date", Time::printHTTPDate());
			cgi.response.processStage |= Response::DONE;
		}
		catch (const ErrorCode &e) {
			cgi.response = e;
		}
		delete &cgi;
		cgis.erase(cgiIt);
	}
}

void	Driver::generateResponse(std::map<int, Client>::iterator& clientIt, std::set<Client *>& activeClients)
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
		logger.logResponse(response, client);

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
				if (client.timer == 0 || client.timer->getState() != ClientTimerState::KEEP_ALIVE)
				{
					delete client.timer;
					client.timer = new KeepAliveState();
				}
				activeClients.insert(&client);
			}
		}
	}
}

void	Driver::updateEpollTimeout()
{
	std::priority_queue<std::time_t>	timeoutPriority;

	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		const Client	&client = it->second;

		if (client.timer != 0)
		{
			timeoutPriority.push(client.timer->getTimeoutTime());
		}
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

	logger.logConnection(logFlag, fd, client);
	epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, 0);
	close(fd);
	establishedSockets.erase(establishedSockets.find(fd));
	clients.erase(clientIt);
}

void	Driver::monitorConnections()
{
	std::map<int, Client>::iterator	it = clients.begin();

	while (it != clients.end())
	{
		Client	&client = it->second;

		if (client.timer->isTimeout(*client.server) == true)
		{
			closeConnection(it++, client.timer->getLogState());
		}
		else
		{
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
