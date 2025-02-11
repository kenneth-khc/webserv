/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/11 03:32:52 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <queue>
#include <vector>
#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "Client.hpp"
#include "MediaType.hpp"

class	Logger;

class	Server
{
public:
	Server();
	~Server();

	/* Server configuration, initialization, event loop */
	void			startListening();
	void			initEpoll();
	int				epollWait();
	void			acceptNewClient();
	void			processReadyEvents();

	ssize_t			receiveBytes(Client&);
	void			processMessages();
	void			processReadyRequests();
	void			generateResponses();

	void			monitorConnections();

	//	TODO: Change access specifier?
	//	Only include a server-wide mapping for now..,
	//	Nginx allows defining in different http, server, and location blocks
	MediaType		map;

	/* HTTP requests */
	Request			receiveRequest(int fd) const;
	Response		handleRequest(const Request&) const;

	/* Handling HTTP methods */
	void			get(Response&, const Request&) const;
	void			post(Response&, const Request&) const;
	void			delete_(Response&, const Request&) const;
	void			put(Response&, const Request&) const;
	void			head(Response&, const Request&) const;

	std::string		getFileContents(const std::string&) const;
	void			generateDirectoryListing(Response&, const std::string&) const;
	void			sendResponse(int socketFD, Response&) const;

	//	TODO: Change access specifier?
	/* Server-wide connection timeout value in seconds */
	static const unsigned int	timeoutValue;

private:
	int				epollFD; // fd of the epoll instance

	std::string		hostName;
	int				portNum;
	int				listenerSocketFD;
	int				numClients; // how many total clients that came through

	int				maxEvents; // how many events to handle each poll
	epoll_event*	readyEvents; // the array of ready events
	int				numReadyEvents; // how many events are ready after a poll

	std::map<std::string,std::string>	directoryMappings;

	std::map<int, Client>	clients;

	friend class Logger;
	Logger			logger;

	// TODO:
	std::queue<Request>		readyRequests;
	std::queue<Response>	readyResponses;

	/* Disallow copying a server */
	Server(Server const&);
	Server&	operator=(Server const&);
};

#endif
