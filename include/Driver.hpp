/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:15:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 08:16:23 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "Configuration.hpp"
#include "HTTP.hpp"
#include "Client.hpp"
#include "Socket.hpp"
#include "String.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include <sys/epoll.h>
#include <sys/types.h>
#include <queue>
#include <map>

extern "C" char	**environ;

struct	Driver
{
	Driver(const Configuration&);
	~Driver();

	String					webServerName;
	struct HTTP				http;

	int						epollFD;
	int						maxEvents;
	epoll_event*			readyEvents;
	int						numReadyEvents;

	std::map<int, Socket>	listeners;
	std::map<int, Socket>	establishedSockets;
	std::map<int, Client>	clients;
	std::queue<Request>		readyRequests;
	std::queue<Response>	readyResponses;

	friend class Logger;
	Logger	logger;

	int			epollWait();
	void		acceptNewClient(const Socket&);
	ssize_t		receiveBytes(Client&);
	Request		receiveRequest(int fd) const;

	void		processReadyEvents();
	void		processMessages();
	void		processReadyRequests();
	void		generateResponses();

	void		monitorConnections();

	friend class CGI;

private:
	/* No default construction or copying necessary */
	Driver();
	Driver(const Driver&);
	Driver&	operator=(const Driver&);

	void				addToEpoll(int, EPOLL_EVENTS);
	// TODO: check these 2 out
	Optional<Server*>	matchServerName(const String&);
	Optional<Server*>	matchPort(int);
};

#endif
