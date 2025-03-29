/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:59:28 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/29 21:54:49 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <netdb.h>
<<<<<<< Updated upstream
#include "Client.hpp"
#include "Socket.hpp"
=======
>>>>>>> Stashed changes

/* This class is responsible for logging interactions between the servers
 * and clients to file streams for visualization purposes */

// TODO: expand this into logging errors encountered?

class	Client;
class	Request;
class	Response;

class	Logger
{
public:
	void	logRequest(Request&, Client&) const;
	void	logResponse(Response&, Client&) const;
	void	logConnection(int, int, Client&) const;

	enum CONNECTION {
		ESTABLISHED,
		KEEP_ALIVE_TIMEOUT,
		CLIENT_HEADER_TIMEOUT,
		CLIENT_BODY_TIMEOUT,
		PEER_CLOSE,
		CLOSE
	};
private:
	static const char*	GREEN;
	static const char*	YELLOW;
	static const char*	RED;
	static const char*	CRESET;

	static void	logIPPort(sockaddr*);
};

#endif
