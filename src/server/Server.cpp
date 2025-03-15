/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/16 01:48:17 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include "Server.hpp"

const unsigned int	Server::timeoutValue = 5;

Server::Server():
domainNames(),
socket()
{

}

Server::Server(std::vector<String> domainNames, Socket* socket):
domainNames(domainNames),
socket(socket),
root()
{
}

void	Server::configureLocations(const Directive& directive)
{
	std::vector<Directive*>	locationBlocks = directive.getDirectives("location");

	for (size_t i = 0; i < locationBlocks.size(); ++i)
	{
		const Directive*	locationBlock = locationBlocks[i];
		Location			location;

		location.root = locationBlock->recursivelyGetValue("root")
									  .value_or("html");


	}
}
