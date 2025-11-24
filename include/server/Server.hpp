/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/24 07:50:45 by cteoh            ###   ########.fr       */
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
#include "PathHandler.hpp"
#include "CGI.hpp"
#include "CGIScriptBlock.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

/**
	This represents the configuration of a virtual host to handle requests
	for specific domain names..
	It tracks all the sockets bound here, all the domain names that this
	should serve, and defines locations to route to based on the incoming
	request.

	More specific directives, such as root, autoindex, error_pages etc can be
	declared here so that it encloses over multiple location scopes and they
	can all inherit from above rather than having to redeclare the directive
	multiple times.

	After resolving the correct location to route to for the request, the
	server executes the necessary procedure to prepare a response for the
	client.
*/

struct Driver;

struct	Server
{
public:
	/** Construct a Server from a directive, binding and reusing sockets
		from the map if necessary */
	Server(const Directive& serverDirective, std::map<int,Socket>& sockets);

	/* Server-wide timeout values in seconds */
	static const unsigned int	keepAliveTimeoutDuration;
	static const unsigned int	cgiTimeoutDuration;
	static const unsigned int	clientHeaderTimeoutDuration;
	static const unsigned int	clientBodyTimeoutDuration;

	/** List of sockets for this server block */
	std::vector<Socket*>	sockets;

	/** Domain names served by this virtual host */
	std::vector<String>		domainNames;

	/** Locations to match and route a request to */
	std::vector<Location>	locations;

	/** Information for handling the request and response */
	String				root;
	std::vector<String>	indexFiles;
	bool				autoindex;
	std::map<int,String>errorPages;
	MediaType			MIMEMappings;
	std::size_t			clientMaxBodySize;

	static const Location		defaultLocation;
	static PathHandler			pathHandler;

	void						handleRequest(Driver&, Client&, Request&,
												Response&, std::set<Timer*>&) const;
	CGI*						handleCGI(Driver&, Client&, Request&, Response&) const;
	void						processCookies(Request&, Response&) const;
	Optional<const Location*>	matchURILocation(const Request&) const;

	/* Handling HTTP methods */
	void				get(Response&, const Request&) const;
	void				post(Response&, const Request&) const;
	void				delete_(Response&, const Request&) const;


	friend class CGI;
	std::vector<CGIScriptBlock>	cgiScriptBlocks;

private:
	Server();

	std::vector<Location>	configureLocations(const Directive&) const;
	/** Binds socket if not already bound, otherwise reuse existing sockets */
	void	assignSocket(const String&, const String&, std::map<int,Socket>&);
};

#endif
