/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:10:19 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/19 22:51:37 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include "String.hpp"
#include "Optional.hpp"

struct	LocationConfig
{
	String	root;
};

struct	ServerConfig
{
	String	serverName;
	short	port;

	std::vector<LocationConfig>	locations;
};

struct	HTTPConfig
{
	std::vector<ServerConfig>	servers;
};

/* The global configurations */
struct	Configuration
{
	Configuration();

	String		prefix;
	HTTPConfig	http;
};

#endif
