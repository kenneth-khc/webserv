/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 06:41:11 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include <sys/epoll.h>
#include "ErrorCode.hpp"
#include <unistd.h>

int	main()
{
	Server	server;

	server.startListening();
	server.initEpoll();

	std::cout << "Server is running...\n";
	while (1)
	{
		if (server.epollWait() != 0)
		{
			server.processReadyEvents();
			server.processMessages();
			server.processReadyRequests();
			server.generateResponses();
		}
		server.monitorConnections();
	}
}
