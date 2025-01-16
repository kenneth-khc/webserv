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
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "debugUtils.hpp"

void	error(const std::string& errmsg)
{
	std::cerr << errmsg << '\n';
	std::exit(1);
}

/* Sets up a socket to start listening on a port,
 * returns the fd of the LISTENING socket */
int	startListening()
{
	addrinfo*	localhost = NULL;
	addrinfo	requirements = {};
	requirements.ai_flags |= AI_CANONNAME | AI_PASSIVE;

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

	int	sock_fd = socket(localhost->ai_family, localhost->ai_socktype, 0);
	if (sock_fd == -1)
	{
	// TODO: do we throw exceptions?
		error("socket failed");
	}
	int	yes = 1;
	retval = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (retval != 0)
	{
		perror("setsockopt");
		error("setsockopt failed");
	}
	retval = bind(sock_fd, localhost->ai_addr, sizeof *localhost->ai_addr);
	if (retval != 0)
	{
		perror("bind");
		error("bind failed");
	}
	retval = listen(sock_fd, 1);
	if (retval != 0)
	{
		error("listen failed");
	}
	freeaddrinfo(localhost);
	return sock_fd;
}

void	communicate(int clientFD)
{
	char	readBuf[128] = {};
	send(clientFD, "Welcome in!\n", sizeof("Welcome in!\n"), 0);
	ssize_t	retval = recv(clientFD, readBuf, sizeof readBuf, 0);
	while (retval > 0)
	{
		std::cout << "Received: " << readBuf << '\n';
		retval = recv(clientFD, readBuf, sizeof readBuf, 0);
	}
}

void	server()
{
	int	sock_fd = startListening();
	sockaddr_storage	clientAddr = {};
	socklen_t			clientAddrLen = 0;

	dbg::println("Server is running...");
	while (1)
	{
		int	clientFD = accept(sock_fd, (sockaddr*)&clientAddr, &clientAddrLen); // blocked here
		dbg::println("someone came in!");
		communicate(clientFD);
	}
}
