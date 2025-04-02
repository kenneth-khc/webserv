/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:59:28 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 20:12:07 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <netdb.h>
#include "Client.hpp"
#include "Token.hpp"
#include "Diagnostic.hpp"

/* This class is responsible for logging interactions between the servers
 * and clients to file streams for visualization purposes */

// TODO: expand this into logging errors encountered?

namespace	Logger
{
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

	extern const char*	filename;

	void	logIPPort(sockaddr*);

	/* Logging configuration errors */
	void	formatErrorMessage(std::stringstream&, const String&);
	void	showErrorLine(std::stringstream&, const Token&);
	void	showErrorLine(std::stringstream&, const Diagnostic&);
	String	getLineFromFile(size_t lineNum, const char*);
	void	seekToLineNum(size_t lineNum, std::ifstream&);

};

#endif
