/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:00:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 20:19:57 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "Socket.hpp"

namespace Logger
{
	namespace Colour
	{
		const char*	RED = "\e[0;31m";
		const char* PURPLE ="\e[0;35m";
		const char*	BOLD_RED = "\e[1;31m";
		const char*	GREEN = "\e[0;32m";
		const char*	YELLOW = "\e[0;33m";
		const char*	BOLD_WHITE = "\x1B[1;37m";
		const char*	BOLD_BLUE = "\x1B[1;34m";
		const char*	RESET = "\e[0m";
	}
}

void	Logger::log(const String& message)
{
	std::cout << Colour::YELLOW << message << '\n' << Colour::RESET;
}

void	Logger::warn(const String& message)
{
	std::cerr << Colour::PURPLE << "WARN: " << message << '\n' << Colour::RESET;
}

void	Logger::warn(const Socket& socket, const String& message)
{
	std::cerr << Colour::YELLOW
			  << "?.?.?.?:????? => "
			  << socket.ip << ":" << socket.port
			  << Colour::RESET
			  << " | "
			  << Colour::PURPLE
			  << message
			  << '\n' << Colour::RESET;
}

void	Logger::logRequest(Request& request, Client& client) 
{
	std::cout << Colour::YELLOW;
	std::cout << client.socket->ip << ":" << client.socket->port;
	std::cout << " => ";
	std::cout << client.receivedBy->ip << ":" << client.receivedBy->port
			  << Colour::RESET
			  << " | ";
	std::cout << Colour::GREEN;
	std::cout << request.method;
	std::cout << ' ' << request.requestTarget << ' ' << "HTTP/" << request.httpVersion;
	std::cout << " | " << request["User-Agent"].value << Colour::RESET;
	std::cout << "\n";
}

void	Logger::logResponse(Response& response, Client& client)
{
	std::cout << Colour::YELLOW;
	std::cout << client.socket->ip << ":" << client.socket->port;
	std::cout << " <= ";
	std::cout << client.receivedBy->ip << ":" << client.receivedBy->port
			  << Colour::RESET
			  << " | ";
	std::cout << Colour::GREEN;
	std::cout << "HTTP/" << response.httpVersion << " "
			  << response.statusCode << " "
			  << response.reasonPhrase << Colour::RESET;
	std::cout << "\n";
}

void	Logger::logConnection(int status, int fd, Client& client)
{
	std::cout << Colour::YELLOW;
	std::cout << client.socket->ip << ":" << client.socket->port;
	std::cout << " <> ";
	std::cout << client.receivedBy->ip << ":" << client.receivedBy->port
			  << Colour::RESET
			  << " | ";
	std::cout << Colour::RED
			  << "FD " << fd << " ";
	switch (status) {
		case Logger::ESTABLISHED:
			std::cout << "Established" << Colour::RESET;
			break ;

		case Logger::KEEP_ALIVE_TIMEOUT:
			std::cout << "Keep Alive Timeout" << Colour::RESET;
			break ;

		case Logger::CLIENT_HEADER_TIMEOUT:
			std::cout << "Client Header Timeout" << Colour::RESET;
			break ;

		case Logger::CLIENT_BODY_TIMEOUT:
			std::cout << "Client Body Timeout" << Colour::RESET;
			break ;

		case Logger::PEER_CLOSE:
			std::cout << "Closed by client" << Colour::RESET;
			break ;

		case Logger::CLOSE:
			std::cout << "Ended" << Colour::RESET;
	}
	std::cout << "\n";
}
