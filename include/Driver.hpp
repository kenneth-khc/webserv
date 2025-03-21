/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Driver.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:15:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/07 22:52:42 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "MediaType.hpp"
#include "Logger.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"
#include <map>
#include <queue>

extern "C" char	**environ;

struct	Driver
{
	Driver();

	String					name;
	std::vector<Server>		servers;

	int						epollFD;
	int						maxEvents;
	epoll_event*			readyEvents;
	int						numReadyEvents;

	std::map<int, Socket>	listeners;
	std::map<int, Client>	clients;
	std::queue<Request>		readyRequests;
	std::queue<Response>	readyResponses;


	//	TODO: Change access specifier?
	//	Only include a server-wide mapping for now..,
	//	Nginx allows defining in different http, server, and location blocks
	MediaType				MIMEMappings;

	//	TODO: Change access specifier?
	/* Server-wide connection timeout value in seconds */
	static const unsigned int	timeoutValue;

	friend class Logger;
	Logger	logger;

	void		configureFrom(const Configuration&);
	void		configNewServer(const Directive&);

	int			epollWait();
	void		acceptNewClient(const Socket&);
	ssize_t		receiveBytes(Client&);
	Request		receiveRequest(int fd) const;
	Response	handleRequest(Request&);

	void		processReadyEvents();
	void		processMessages();
	void		processReadyRequests();
	void		generateResponses();

	void		processCookies(Request&, Response&);
	void		monitorConnections();

	/* Handling HTTP methods */
	void		get(Response&, Request&) const;
	void		post(Response&, Request&) const;
	void		delete_(Response&, Request&) const;

	void		cgi(Response&, const Request&) const;
	void		generateDirectoryListing(Response&, const std::string&) const;

	// Resources Directories
	const String 	rootDir;
	const String 	pagesDir;
	const String 	uploadsDir;
	const String 	miscPagesDir;
	const String	cgiDir;

	bool				autoindex;

	std::vector<String>	cgiScript;

	std::map<std::string,std::string>	directoryMappings;

	friend class CGI;

};

#endif
