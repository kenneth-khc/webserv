/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/19 17:52:51 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include "debugUtils.hpp"

void	server();

int	main()
{
	Server	server;

	server.startListening();
	server.initEpoll();

	std::cout << "Server is running...\n";
	while (1)
	{
		dbg::println("Polling...");
		server.epollWait();
		server.processReadyEvents();
	}
}
