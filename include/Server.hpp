/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 07:44:25 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "String.hpp"
#include "Socket.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "MediaType.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

struct	Server
{
	Server();
	Server(const Directive&, std::map<int,Socket>&);

	Socket*					socket;
	std::vector<String>		domainNames;

	String					root;
	std::vector<Location>	locations;
	bool					autoindex;
	std::vector<String>		indexFiles;
	MediaType				MIMEMappings;
	std::size_t				clientMaxBodySize;
	std::map<int,String>	errorPages;

	static const Location		defaultLocation;
	static const unsigned int	timeoutValue;

	Response					handleRequest(Request&);
	void						processCookies(Request&, Response&);
	Optional<const Location*>	matchURILocation(const Request&);

	/* Handling HTTP methods */
	void		get(Response&, Request&, const Location&) const;
	void		post(Response&, Request&) const;
	void		delete_(Response&, Request&) const;
	
	void		cgi(Response&, const Request&) const;

	// TODO: dumped this here just to compile, fix it
	std::vector<String>	cgiScript;

private:
	void	checkIfAllowedMethod(const Location&, const Request&);
	void	configureLocations(const Directive&);
	void	assignSocket(const String&, const String&, std::map<int,Socket>&);
};

// functor this functor that I hate C++98 go func yourself
struct isDefaultListener
{
	const Socket*	listener;

	isDefaultListener(const Socket* listener): listener(listener) {}
	bool	operator()(const Server& server)
	{
		return server.socket == listener;
	}
};

#endif
