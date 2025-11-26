/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTP.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 06:58:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 08:16:17 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

/* This encompasses all the configuration options related to HTTP, such as
 * the servers to serve files, the root directory to look for files, the index
 * files, whether autoindex gets turned on, the types that our transfer should
 * support, etc...
 *
 * HTTP Servers that are created within will inherit values from HTTP if they
 * themselves do not override it.
 *
 * It is a disconnect from lower-level technicalities, such as the polling
 * and socketing that enables the transfer. This class shouldn't need to know
 * about any of that. */

#include "Directive.hpp"
#include "Server.hpp"
#include "MediaType.hpp"
#include "String.hpp"
#include <vector>

struct HTTP
{
	HTTP();
	/* Read configuration options from a HTTP Directive */
	HTTP(const Directive&);
	HTTP&	operator=(const HTTP&);

	std::vector<Server>	servers;
	MediaType			MIMEMappings;
	bool				autoindex;
	String				rootDirectory;
	std::vector<String>	indexFiles;
	std::size_t			clientMaxBodySize;
	std::map<int,String>errorPages;

	/* Insert a Server into the HTTP scope */
	void	addServer(const Server&);

private:
	// No default construction or copying necessary
	HTTP(const HTTP&);
};

#endif
