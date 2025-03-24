/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/25 01:00:25 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "String.hpp"
#include "Socket.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <map>

struct	Server
{
	Server();
	Server(std::vector<String>, int, Socket*);

	std::vector<String>	domainNames;
	int					portNum;
	Socket*				socket;
	int					socketFD;
	int					numClients;

	/* Server-wide timeout values in seconds */
	static const unsigned int	keepAliveTimeout;
	static const unsigned int	cgiTimeout;
	static const unsigned int	clientHeaderTimeout;
	static const unsigned int	clientBodyTimeout;

	String					root;
	std::map<String,String>	routes;
};

#endif
