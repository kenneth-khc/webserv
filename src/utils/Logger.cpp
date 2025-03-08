/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:00:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/05 22:33:14 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Server.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"

const char*	Logger::GREEN = "\e[0;32m";
const char*	Logger::YELLOW = "\e[0;33m";
const char*	Logger::RED = "\e[0;31m";
const char*	Logger::CRESET = "\e[0m";

void	Logger::logIPPort(sockaddr* client)
{
	if (client->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(client);
		char			ipv4[INET_ADDRSTRLEN];
		unsigned short	portNum = ntohs(addr->sin_port);
		inet_ntop(AF_INET, &addr->sin_addr, ipv4, INET_ADDRSTRLEN);
		std::cout << ipv4 << ":" << portNum;
	}
	else if (client->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(client);
		char			ipv6[INET6_ADDRSTRLEN];
		unsigned short	portNum = ntohs(addr->sin6_port);
		inet_ntop(AF_INET6, &addr->sin6_addr, ipv6, INET6_ADDRSTRLEN);
		std::cout << ipv6 << ":" << portNum;
	}
}

// TODO: clean this up nicely
void	Logger::logRequest(Request& request, Client& client) const
{
	std::cout << YELLOW;
	std::cout << client.getIPAddr() << ":" << client.getPortNum();
	std::cout << " => ";
	std::cout 
			  //<< server.hostName 
			  << ":"
			  << client.socket.port  << CRESET
			  << " | ";
	std::cout << GREEN;
	std::cout << request.method;
	std::cout << ' ' << request.requestTarget << ' ' << "HTTP/" << request.httpVersion;
	std::cout << " | " << request["User-Agent"].value << CRESET;
	std::cout << "\n";
}

void	Logger::logResponse(Response& response, Client& client) const
{
	std::cout << YELLOW;
	std::cout << client.getIPAddr() << ":" << client.getPortNum();
	std::cout << " <= ";
	std::cout 
			  //<< server.hostName << ":"
			  << client.socket.port  << CRESET
			  << " | ";
	std::cout << GREEN;
	std::cout << "HTTP/" << response.httpVersion << " "
			  << response.statusCode << " "
			  << response.reasonPhrase << CRESET;
	std::cout << "\n";
}

void	Logger::logConnection(int status, int fd, Client& client) const
{
	std::cout << YELLOW;
	std::cout << client.getIPAddr() << ":" << client.getPortNum();
	std::cout << " <> ";
	std::cout //<< ":"
			  << client.socket.port  << CRESET
			  << " | ";
	std::cout << RED
			  << "FD " << fd << " ";
	if (status == Logger::ESTABLISHED)
	{
		std::cout << "Established" << CRESET;
	}
	else if (status == Logger::TIMEOUT)
	{
		std::cout << "Timeout" << CRESET;
	}
	else if (status == Logger::CLOSE)
	{
		std::cout << "Closed by client" << CRESET;
	}
	std::cout << "\n";
}
