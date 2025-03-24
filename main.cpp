/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/24 09:39:34 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include "Configuration.hpp"
#include "Parser.hpp"
#include "Driver.hpp"
#include "ErrorCode.hpp"

void	sigint_exit(int)
{
	std::cout << "\nGot a SIGINT, exiting!\n";
	std::exit(1);
}

int	main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error. Usage: ./webserv <config_file>\n";
		std::exit(1);
	}

	// TODO: this is just for debugging the server so that it exits
	//		 cleanly when we CTRL-C, remove later
	std::signal(SIGINT, sigint_exit);

	Parser			parser(argv[1]);
	Configuration	config = parser.parseConfig();
	config.display();

	/* with all the configuration values successfully validated, we can now
	 * start setting up what is necessary to run our webserver */

	Driver	driver(config);

	std::cout << "Server is running...\n";
	while (1)
	{
		if (driver.epollWait() != 0)
		{
			driver.processReadyEvents();
			driver.processMessages();
			driver.processReadyRequests();
			driver.generateResponses();
		}
		driver.monitorConnections();
	}
}
