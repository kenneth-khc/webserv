/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/27 02:07:15 by cteoh            ###   ########.fr       */
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

const unsigned int	Server::keepAliveTimeout = 5;
const unsigned int	Server::cgiTimeout = 3;
const unsigned int	Server::clientHeaderTimeout = 2;
const unsigned int	Server::clientBodyTimeout = 2;

Server::Server():
domainNames(),
portNum(0),
socketFD(-1),
numClients(0),
routes()
{

}

Server::Server(std::vector<String> domainNames, int portNum, Socket* socket):
domainNames(domainNames),
portNum(portNum),
socket(socket),
socketFD(socket->fd),
numClients(0),
root(),
routes()
{
}
