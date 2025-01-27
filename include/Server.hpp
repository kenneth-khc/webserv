/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/27 23:49:05 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/epoll.h>
#include "Request.hpp"
#include "Response.hpp"

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

	/* HTTP requests */
	std::string		receiveRequest(int fd) const;
	Response		handleRequest(const Request&) const;

	/* Handling HTTP methods */
	void			get(Response&, const Request&) const;

	std::string		getFileContents(const std::string&) const;
	void			sendResponse(int socketFD, const Response&) const;
	

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


	/* Disallow copying a server */
	Server(Server const&);
	Server&	operator=(Server const&);
};

#endif
