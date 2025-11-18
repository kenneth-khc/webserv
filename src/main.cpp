/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 17:37:28 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include "Configuration.hpp"
#include "Parser.hpp"
#include "Driver.hpp"
#include "SetupError.hpp"

int	main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error. Usage: ./webserv <config_file>\n";
		std::exit(1);
	}

	Parser			parser(argv[1]);
	Configuration	config = parser.parseConfig();
	config.display();

	Driver	driver;
	try
	{
		driver.initialize(config);
	}
	catch (const SetupError& e)
	{
		std::cout << e.format();
		std::exit(1);
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << '\n';
		std::exit(1);
	}

	std::cout << "Server is running...\n";
	while (1)
	{
		driver.updateEpollTimeout();
		if (driver.epollWait() != 0)
		{
			driver.processReadyEvents();
		}
		driver.monitorTimers();
	}
}
