/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/01 09:43:51 by kecheong         ###   ########.fr       */
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

class	Logger;

class	Server
{
public:
	Server();
	~Server();

	/* Server configuration, initialization, event loop */
	void			startListening();
	void			initEpoll();
	void			epollWait();
	void			acceptNewClient();
	void			processReadyEvents();

	ssize_t			receiveBytes(Client&);
	void			processMessages();
	void			processReadyRequests();
	void			generateResponses();

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
