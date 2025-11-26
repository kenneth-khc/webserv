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
#include "ConfigError.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include "Driver.hpp"
#include "SetupError.hpp"

namespace
{
	volatile std::sig_atomic_t	serverIsRunning = 1;
}

void	handleSigInt(int signalCode)
{
	serverIsRunning = 0;
	Logger::log("Server is shutting down after receiving signal " +
				String(signalCode) + " (SIGINT) ");
}

void	handleSigTerm(int signalCode)
{
	serverIsRunning = 0;
	Logger::log("Server is terminating after receiving signal " +
	            String(signalCode) + " (SIGTERM)");
}

int	main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error. Usage: ./webserv <config_file>\n";
		return EXIT_FAILURE;
	}

	Parser			parser(argv[1]);
	Configuration*	config = NULL;
	try
	{
		config = parser.parseConfig();
	}
	catch (const ConfigError& e)
	{
		std::cerr << e.format();
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Caught unhandled exception: " << e.what() << '\n';
		return EXIT_FAILURE;
	}

	Driver	driver;
	try
	{
		driver.initialize(*config);
		delete config;
	}
	catch (const SetupError& e)
	{
		std::cerr << e.format();
		delete config;
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Caught unhandled exception: " << e.what() << '\n';
		delete config;
		return EXIT_FAILURE;
	}

	std::signal(SIGINT, handleSigInt);
	std::signal(SIGTERM, handleSigTerm);
	Logger::log("Server is running with " + String(driver.listeners.size()) +
				" listening sockets");
	while (serverIsRunning)
	{
		driver.updateEpollTimeout();
		if (driver.epollWait() != 0)
		{
			driver.processReadyEvents();
		}
		driver.monitorTimers();
	}
	return EXIT_SUCCESS;
}
