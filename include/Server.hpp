/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/06 23:14:21 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "String.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <map>

struct	Server
{
	Server();
	Server(String, int);

	String	name;
	int		portNum;
	int		socketFD;
	int		numClients;

	static const unsigned int	timeoutValue;

	std::map<String,String>	routes;
};

#endif
