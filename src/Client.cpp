/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:40:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 15:52:14 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include "Client.hpp"

// TODO: limit request size based on config file. now defaulted to 1 mb
const size_t	Client::MAX_REQUEST_SIZE = 1 * 1024 * 1024;

Client::Client():
socketFD(),
address(),
addressLen(),
messageBuffer(),
message(),
request(),
firstDataRecv(false),
lastActive(std::time(0))
{
	messageBuffer.resize(1024);
}

bool	Client::isTimeout() const
{
	if (firstDataRecv == true && (std::time(0) - lastActive >= Server::timeoutValue))
		return (true);
	return (false);
}

void	Client::updateLastActive()
{
	lastActive = std::time(0);
}
