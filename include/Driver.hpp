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
#include <set>
#include <vector>

extern "C" char	**environ;

class CGI;

struct	Driver
{
	Driver();

	String					name;
	std::vector<Server>		servers;

	int						epollFD;
	int						maxEvents;
	epoll_event*			readyEvents;
	int						numReadyEvents;
	epoll_event				*currEvent;

	std::map<int, Socket>	listeners;
	std::map<int, Client>	clients;
	std::map<int, CGI *>	cgis;

	//	TODO: Change access specifier?
	//	Only include a server-wide mapping for now..,
	//	Nginx allows defining in different http, server, and location blocks
	MediaType				MIMEMappings;

	friend class Logger;
	Logger	logger;

	void		configureFrom(const Configuration&);
	void		configNewServer(const Directive&);

	int			epollWait();
	void		acceptNewClient(const Socket&);

	void		processReadyEvents();
	void		receiveMessage(std::map<int, Client>::iterator&);
	void		processRequest(std::map<int, Client>::iterator&, std::set<Client *>&);
	void		processCGI(std::map<int, CGI*>::iterator&);
	void		processReadyRequest(Request&, Response&);
	void		generateResponse(std::map<int, Client>::iterator&);

	void		processCookies(Request&, Response&);
	void		closeConnection(std::map<int, Client>::iterator, int);
	void		monitorConnections();

	/* Handling HTTP methods */
	void		get(Request&, Response&);
	void		post(Request&, Response&);
	void		delete_(Request&, Response&) const;

	void		cgi(Request&, Response&);
	void		generateDirectoryListing(Response&, const String&) const;
	void		generateUploadsListing(const Request&, Response&, const String&) const;

	// Resources Directories
	const String 	rootDir;
	const String 	pagesDir;
	const String 	uploadsDir;
	const String 	miscPagesDir;
	const String	cgiDir;

	bool				autoindex;
	bool				uploads;

	std::vector<String>	cgiScript;

	std::map<std::string,std::string>	directoryMappings;

	friend class CGI;

};

#endif
