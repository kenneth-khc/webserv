/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:41:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/21 07:29:17 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Driver.hpp"
#include "PathHandler.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Configuration.hpp"
#include "ErrorCode.hpp"
#include "DoneState.hpp"
#include "KeepAliveTimer.hpp"
#include <cstddef>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <algorithm>
#include <cstdio>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <queue>
#include <vector>
#include <algorithm>

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
	if (numReadyEvents == -1)
	{
		throw std::runtime_error("epoll_wait() failed");
	}
	return numReadyEvents;
}

/*
   Based on the number of ready events returned by epoll_wait(), goes through
   each event and process them accordingly. Finally, update the timeout values
   for those event if applicable.
*/
void	Driver::processReadyEvents()
{
	std::set<Timer*>	activeTimers;

	for (int i = 0; i < numReadyEvents; ++i)
	{
		currEvent = &readyEvents[i];
		const int	fd = currEvent->data.fd;
		std::map<int,Socket>::iterator	iter = listeners.find(fd);

		// If the event originates from a listener socket, creates a client
		// socket.
		if (iter != listeners.end())
		{
			const Socket&	listener = iter->second;
			Socket			clientSocket = listener.accept();
			int				fd = clientSocket.fd;
			establishedSockets[fd] = clientSocket;
			addToEpoll(fd, EPOLL_EVENTS(EPOLLIN | EPOLLOUT));

			Client	newClient(&establishedSockets[fd], &listener);
			for (size_t i = 0; i < http.servers.size(); ++i)
			{
				const Server& server = http.servers[i];
				// TODO(kecheong):
				// go through each socket of each server
				if (&listener == server.socket)
				{
					newClient.defaultServer = &server;
					break;
				}
			}
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
			// If the event originates from a client socket, stores the request
			// or sends a ready response.

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
			// If the event originates from a CGI pipe, sends the necessary CGI
			// data or store and process its response.

			processCGI(cgiIt, activeTimers);
		}
	}

	// Updates timeout values for client or CGI.
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

const Server*	Driver::selectVirtualHost(const std::vector<Server>& servers,
										  const Client& client,
										  const Request& request)
{
	String	host = request.find< Optional<String> >("Host").value_or("");
	if (host.find(':'))
	{
		host = host.consumeUntil(":").value;
	}
	std::vector<Server>::const_iterator	server;
	for (server = servers.begin(); server != servers.end(); ++server)
	{
		if (server->socket != client.receivedBy)
		{
			continue;
		}
		if (std::find(server->domainNames.begin(),
					  server->domainNames.end(),
					  host) != server->domainNames.end())
		{
			return &*server;
		}
	}
	return client.defaultServer;
}

/*
   Processes each client socket's message as it is incoming. When parts of the
   HTTP request is determined to be valid, processes that specific section and
   moves on to the next.
*/
void	Driver::processRequest(std::map<int, Client>::iterator& clientIt,
							   std::set<Timer*> activeTimers)
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
			while (request.processState(client)->getState() != RequestState::DONE)
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

			const Server*	server = selectVirtualHost(http.servers,
													   client,
													   request);
			server->handleRequest(request, client.responseQueue.back());
		}
		catch (const ErrorCode &e)
		{
			delete request.state;
			request.state = new DoneState();
			request.processState(client);
			client.responseQueue.back() = e;
		}

		client.requestQueue.push_back(Request());
		if (client.responseQueue.back()["Connection"].value == "close")
		{
			return ;
		}
	}
}

void	Driver::processCGI(std::map<int, CGI*>::iterator& cgiIt,
						   std::set<Timer*> activeTimers)
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
				std::remove(client.cgis.begin(), client.cgis.end(), &cgi);
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
		std::remove(client.cgis.begin(), client.cgis.end(), &cgi);
		delete &cgi;
	}
}

/*
	Checks whether a response ready (has status line and headers). For
	responses with message body, the body will also be appended (this occurs)
	continuously if the entire body is not ready yet).
*/
void	Driver::sendResponse(std::map<int, Client>::iterator& clientIt,
							 std::set<Timer*> activeTimers)
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

/*
   Finds the timer will expire the soonest and update the epoll timeout to that
   value.
*/
void	Driver::updateEpollTimeout()
{
	std::priority_queue<
		std::time_t,
		std::vector<std::time_t>,
		std::greater<std::time_t> >	timeoutPriority;

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
		delete *cgiIt;
		cgiIt++;
	}

	clients.erase(clientIt);
}

/*
   Checks each available timer and see if they are timed out. If so, close the
   connection, or kill the CGI process and return 500 errror.
*/
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
