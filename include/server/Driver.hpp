/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:15:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/05 15:53:27 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "Configuration.hpp"
#include "HTTP.hpp"
#include "Client.hpp"
#include "Timer.hpp"
#include "Socket.hpp"
#include "String.hpp"
#include "Request.hpp"
#include "Configuration.hpp"
#include "CGI.hpp"
#include <sys/epoll.h>
#include <sys/types.h>
#include <map>

extern "C" char	**environ;

class CGI;

struct	Driver
{
	Driver();
	Driver(const Configuration&);
	~Driver();

	void	initialize(const Configuration&);

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
	std::map<int, CGI*>		cgis;

	int			epollWait();

	void		processReadyEvents();
	void		receiveMessage(std::map<int, Client>::iterator&);
	void		processRequest(std::map<int, Client>::iterator&, std::set<Timer*>&);
	void		processCGI(std::map<int, CGI*>::iterator&, std::set<Timer*>&);
	void		sendResponse(std::map<int, Client>::iterator&, std::set<Timer*>&);

	void		updateEpollTimeout();

	void		monitorTimers();
	void		closeConnection(std::map<int, Client>::iterator, int);

private:
	/* No default construction or copying necessary */
	Driver(const Driver&);
	Driver&	operator=(const Driver&);

	void			addToEpoll(int, EPOLL_EVENTS);
	const Server*	selectVirtualHost(const std::vector<Server>&,
									  const Client&,
									  const Request&);
};

#endif
