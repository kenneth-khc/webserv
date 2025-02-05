/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:00:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 04:04:30 by cteoh            ###   ########.fr       */
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
const char*	Logger::CRESET = "\e[0m";

// TODO: clean this up nicely
void	Logger::logRequest(const Server& server, Request& request) const
{
	sockaddr*	client = (sockaddr*)&request.srcAddress;

	std::cout << YELLOW;
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
	std::cout << " => ";
	std::cout << server.hostName << ":"
			  << server.portNum  << CRESET
			  << " | ";
	std::cout << GREEN;
	for (int i = 0; i < NUM_OF_METHODS; ++i)
	{
		if (request.method == i)
		{
			std::cout << request.methods[i];
		}
	}
	std::cout << ' ' << request.requestTarget << ' ' << "HTTP/" << request.httpVersion;
	std::cout << " | " << request["User-Agent"].value << CRESET;
	std::cout << "\n\n";
}

void	Logger::logResponse(const Server& server, Response& response) const
{
	sockaddr*	client = (sockaddr*)&response.destAddress;

	std::cout << YELLOW;
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
	std::cout << " <= ";
	std::cout << server.hostName << ":"
			  << server.portNum  << CRESET
			  << " | ";
	std::cout << GREEN;
	std::cout << "HTTP/" << response.httpVersion << " "
			  << response.statusCode << " "
			  << response.reasonPhrase << CRESET;
	std::cout << "\n\n";
}
