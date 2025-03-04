/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:15:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/05 13:19:16 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <map>
#include <queue>

struct	Servers
{
	Servers();

	std::vector<Srvr>		servers;
	int						epollFD;
	int						maxEvents;
	epoll_event*			readyEvents;
	int						numReadyEvents;
	std::map<int, Client>	clients;
	std::queue<Request>		readyRequests;
	std::queue<Response>	readyResponses;
	MediaType				map;

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
