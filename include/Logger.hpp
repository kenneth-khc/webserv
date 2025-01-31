/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:59:28 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/31 13:59:46 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

/* This class is responsible for logging interactions between the servers
 * and clients to file streams for visualization purposes */

// TODO: expand this into logging errors encountered?

class	Server;
class	Request;
class	Response;

class	Logger
{
public:
	void	logRequest(const Server&, Request&, int clientFD) const;
	void	logResponse(const Server&, Response&, int clientFD) const;

private:
	static const char*	GREEN;
	static const char*	YELLOW;
	static const char*	CRESET;
};

#endif
