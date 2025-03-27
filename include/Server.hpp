/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/27 16:30:12 by cteoh            ###   ########.fr       */
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
	static const unsigned int	keepAliveTimeoutDuration;
	static const unsigned int	cgiTimeoutDuration;
	static const unsigned int	clientHeaderTimeoutDuration;
	static const unsigned int	clientBodyTimeoutDuration;

	String					root;
	std::map<String,String>	routes;
};

#endif
