/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:48:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/16 20:15:16 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <string>
#include "debugUtils.hpp"

void	error(const std::string& errmsg)
{
	std::cerr << errmsg << '\n';
	std::exit(1);
}

template <typename Type>
std::string	toString(Type const& t)
{
	std::stringstream	ss;
	ss << t;
	return ss.str();
}

/* Sets up a socket to start listening on a port,
 * returns the fd of the LISTENING socket */
int	startListening()
{
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;
	requirements.ai_socktype = SOCK_STREAM;

	// TODO: port 80 requires the process to run with root permission
	int	retval = getaddrinfo("localhost", "8000", &requirements, &localhost);
	if (retval != 0)
	{
		error(gai_strerror(retval));
	}
	else if (localhost == NULL)
	{
		error("no valid address found");
	}
	dbg::printAddrInfos(localhost);

	int	sockFD = socket(localhost->ai_family, localhost->ai_socktype, 0);
	if (sockFD == -1)
	{
	// TODO: do we throw exceptions?
		error("socket failed");
	}
	int	yes = 1;
	retval = setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (retval != 0)
	{
		perror("setsockopt");
		error("setsockopt failed");
	}
	retval = bind(sockFD, localhost->ai_addr, sizeof *localhost->ai_addr);
	if (retval != 0)
	{
		perror("bind");
		error("bind failed");
	}
	retval = listen(sockFD, 1);
	if (retval != 0)
	{
		error("listen failed");
	}
	freeaddrinfo(localhost);
	return sockFD;
}

void	prettyPrintReceived(char* buf, ssize_t bytes)
{
	std::cout << "Received " << bytes << " bytes:\n";
	for (int i = 0; i < bytes; ++i)
		std::cout << '-';
	std::cout << '\n';
	std::cout << buf;
	for (int i = 0; i < bytes; ++i)
		std::cout << '-';
	std::cout << '\n';
}

void	communicate(int clientFD)
{
	char	readBuf[128] = {};
	std::string	buf;
	ssize_t	retval = recv(clientFD, readBuf, sizeof readBuf, 0);
	prettyPrintReceived(readBuf, retval);
	while (retval > 0)
	{
		std::memset(readBuf, 0, sizeof readBuf);
		retval = recv(clientFD, readBuf, sizeof readBuf, 0);
		if (retval == 0)
		{
			std::cout << "Client disconnected\n";
			break ;
		}
		prettyPrintReceived(readBuf, retval);
	}
}

void	server()
{
	int	sockFD = startListening();
	sockaddr_storage	clientAddr = {};
	socklen_t			clientAddrLen = sizeof clientAddr;
	int					clientNum = 0;

	dbg::println("Server is running...");
	while (1)
	{
		int	clientFD = accept(sockFD, (sockaddr*)&clientAddr, &clientAddrLen); // blocked here
		dbg::println("Someone came in!");
		dbg::printSocketAddr((sockaddr*)&clientAddr);
		++clientNum;
		std::string	tmp = "Hello, you are client " + toString(clientNum) + "!\n"
						+ "--------------------------------------------------\n";
		send(clientFD, tmp.data(), tmp.size(), 0);
		communicate(clientFD);
	}
}
