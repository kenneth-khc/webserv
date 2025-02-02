/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/02 05:25:01 by kecheong         ###   ########.fr       */
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
		server.epollWait();
		server.processReadyEvents();
		try
		{
			server.processMessages();
			server.processReadyRequests();
			server.generateResponses();
		}
		catch (const ErrorCode& e)
		{
			std::cout << e.what() << '\n';
		}
	}
}
