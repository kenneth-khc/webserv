/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:59:28 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/01 22:25:54 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

/* This class is responsible for logging interactions between the servers
 * and clients to file streams for visualization purposes */

// TODO: expand this into logging errors encountered?

class	Server;
class	Client;
class	Request;
class	Response;

class	Logger
{
public:
	void	logRequest(const Server&, Request&, Client&) const;
	void	logResponse(const Server&, Response&, Client&) const;
	void	logConnection(const Server&, int, int, Client&) const;

	enum CONNECTION {
		ESTABLISHED,
		TIMEOUT,
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
