/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/16 01:28:23 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "String.hpp"
#include "Socket.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>

struct	Server
{
	Server();
	Server(std::vector<String>, Socket*);

	std::vector<String>	domainNames;
	Socket*				socket;
	/*int					numClients;*/

	static const unsigned int	timeoutValue;

	String					root;

	std::vector<Location>	locations;

	void	configureLocations(const Directive&);
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
