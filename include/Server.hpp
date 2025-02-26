/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/26 22:58:50 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <queue>
#include <vector>
#include "String.hpp"
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

	const String	name;

	// Server-wide connection timeout value in seconds
	static const unsigned int	timeoutValue;
	void			monitorConnections();

	/* HTTP requests */
	Request			receiveRequest(int fd) const;
	Response		handleRequest(Request&);
	void			processCookies(Request&, Response&);

	/* Handling HTTP methods */
	void			get(Response&, const Request&);
	void			post(Response&, const Request&);
	void			delete_(Response&, const Request&) const;

	void			generateDirectoryListing(Response&, const std::string&) const;

private:
	int				epollFD; // fd of the epoll instance

	std::string		hostName;
	int				portNum;
	int				listenerSocketFD;
	int				numClients; // how many total clients that came through

	int				maxEvents; // how many events to handle each poll
	epoll_event*	readyEvents; // the array of ready events
	int				numReadyEvents; // how many events are ready after a poll

	// Only include a server-wide configurations for now..,
	// Nginx allows defining in different http, server, and location blocks
	MediaType		MIMEMappings;

	// Resources Directories
	const String 	rootDir;
	const String 	pagesDir;
	const String 	uploadsDir;
	const String 	errorPagesDir;
	const String 	miscPagesDir;

	bool			autoindex;

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
