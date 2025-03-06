/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:15:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/06 23:14:30 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "MediaType.hpp"
#include "Logger.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"
#include <map>
#include <queue>

struct	Servers
{
	Servers();

	std::vector<Server>		servers;

	int						epollFD;
	int						maxEvents;
	epoll_event*			readyEvents;
	int						numReadyEvents;

	std::map<int, Client>	clients;
	std::queue<Request>		readyRequests;
	std::queue<Response>	readyResponses;

	//	TODO: Change access specifier?
	//	Only include a server-wide mapping for now..,
	//	Nginx allows defining in different http, server, and location blocks
	MediaType				map;

	//	TODO: Change access specifier?
	/* Server-wide connection timeout value in seconds */
	static const unsigned int	timeoutValue;

	friend class Logger;
	Logger	logger;

	void	configureFrom(const Configuration&);
	void	configNewServer(const Directive&);

	int		epollWait();
	void	acceptNewClient(int);
	ssize_t	receiveBytes(Client&);
	Response	handleRequest(const Request&) const;

	void	processReadyEvents();
	void	processMessages();
	void	processReadyRequests();
	void	generateResponses();

	void	monitorConnections();

	/* Handling HTTP methods */
	void			get(Response&, const Request&) const;
	void			post(Response&, const Request&) const;
	void			delete_(Response&, const Request&) const;
	void			put(Response&, const Request&) const;
	void			head(Response&, const Request&) const;

	void			generateDirectoryListing(Response&, const std::string&) const;


};

#endif
