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
#include <fstream>
#include <limits>
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace Logger
{
	namespace Colour
	{
		const char*	RED = "\e[0;31m";
		const char*	BOLD_RED = "\e[1;31m";
		const char*	GREEN = "\e[0;32m";
		const char*	YELLOW = "\e[0;33m";
		const char*	BOLD_WHITE = "\x1B[1;37m";
		const char*	BOLD_BLUE = "\x1B[1;34m";
		const char*	RESET = "\e[0m";
	}
	const char*			configFileName;
	std::stringstream	ss;
}


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

void	Logger::error(const String& errorMsg)
{
	ss << Colour::RED << "error: " << Colour::RESET;
	ss << Colour::BOLD_WHITE << errorMsg << Colour::RESET << '\n';
}

void	Logger::error(const std::stringstream& errorMsg)
{
	ss << Colour::RED << "error: " << Colour::RESET;
	ss << Colour::BOLD_WHITE << errorMsg.str() << Colour::RESET << '\n';
}

void	Logger::formatErrorMessage(std::stringstream& ss, const String& msg)
{
	using namespace Logger::Colour;

	ss << RED << "error: " << RESET;
	ss << BOLD_WHITE << msg << RESET << '\n';
}

String	Logger::getLineFromFile(size_t lineNum, const char* filename)
{
	std::ifstream	file(filename);
	seekToLineNum(lineNum, file);
	std::string		line;
	std::getline(file, line);
	return line;
}

void	Logger::seekToLineNum(size_t lineNum, std::ifstream& file)
{
	file.seekg(std::ios::beg);
	for (size_t i = 0; i < lineNum-1; ++i)
	{
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

template <typename Number>
int	countDigits(Number number)
{
	int	digits = 0;
	if (number < 0)
	{
		digits = 1;
	}
	while (number != 0)
	{
		number /= 10;
		++digits;
	}
	return digits;
}

void	Logger::showErrorLine(const Diagnostic& diagnostic)
{
	using namespace Logger::Colour;

	size_t	lineNum = diagnostic.lineNum;
	size_t	columnNum = diagnostic.columnNum;
	std::string	indent = " ";
	std::string	arrow;
	for (int i = 0, digits = countDigits(lineNum);
		 i < digits;
		 ++i)
	{
		indent += ' ';
		arrow += '-';
	}
	arrow += "> ";

	ss << Colour::BOLD_BLUE << arrow << Colour::RESET 
	   << configFileName << ':'
	   << lineNum << ':' << columnNum << Colour::RESET << '\n';

	const String&	line = getLineFromFile(lineNum, configFileName);
	ss << indent << Colour::BOLD_BLUE << '|' << Colour::RESET << '\n'
	   << Colour::BOLD_BLUE << lineNum << " | " << Colour::RESET << line
	   << '\n';

	ss << indent << BOLD_BLUE << "| " << RESET;
	std::string	padding;
	for (size_t i = 0; i < columnNum-1; ++i)
	{
		if (line[i] == '\t')
		{
			padding += '\t';
		}
		else
		{
			padding += ' ';
		}
	}
	ss << padding << BOLD_RED << '^' << RESET << '\n';
}

String	Logger::streamToString()
{
	return ss.str();
}

