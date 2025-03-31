/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:15:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/28 21:59:58 by kecheong         ###   ########.fr       */
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
#include "Configuration.hpp"
#include "PathHandler.hpp"
#include "Logger.hpp"
#include <sys/epoll.h>
#include <sys/types.h>
#include <set>
#include <vector>
#include <queue>
#include <map>

extern "C" char	**environ;

class CGI;

/*int						globalEpollFD;*/
/*std::map<int, CGI*>*	globalCgis;*/

struct	Driver
{
	Driver(const Configuration&);
	~Driver();

	String					webServerName;
	struct HTTP				http;

	int						epollTimeout;
	int						epollFD;
	int						maxEvents;
	epoll_event*			readyEvents;
	int						numReadyEvents;
	epoll_event				*currEvent;

	std::map<int, Socket>	listeners;
	std::map<int, Socket>	establishedSockets;
	std::map<int, Client>	clients;
	std::map<int, CGI *>	cgis;

	friend class Logger;
	Logger	logger;

	int			epollWait();

	void		processReadyEvents();
	void		receiveMessage(std::map<int, Client>::iterator&);
	void		processRequest(std::map<int, Client>::iterator&, std::set<Client *>&);
	void		processCGI(std::map<int, CGI*>::iterator&);
	void		generateResponse(std::map<int, Client>::iterator&, std::set<Client *>&);

	void		updateEpollTimeout();

	void		monitorConnections();
	void		closeConnection(std::map<int, Client>::iterator, int);

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
