/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:59:28 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/30 00:35:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "String.hpp"

#include <netdb.h>

/* This namespace is responsible for logging interactions between the servers
 * and clients to file streams for visualization purposes */

class Client;
class Request;
class Response;

namespace	Logger
{
	void	log(const String& message);
	void	logRequest(Request&, Client&);
	void	logResponse(Response&, Client&);
	void	logConnection(int, int, Client&);

	enum CONNECTION {
		ESTABLISHED,
		KEEP_ALIVE_TIMEOUT,
		CLIENT_HEADER_TIMEOUT,
		CLIENT_BODY_TIMEOUT,
		PEER_CLOSE,
		CLOSE
	};

	namespace	Colour
	{
		extern const char*	RED;
		extern const char*	BOLD_RED;
		extern const char*	GREEN;
		extern const char*	YELLOW;
		extern const char*	BOLD_WHITE;
		extern const char*	BOLD_BLUE;
		extern const char*	RESET;
	}
};

#endif
