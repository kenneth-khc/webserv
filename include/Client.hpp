/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 15:52:23 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <vector>
#include <ctime>
#include "String.hpp"
#include "Request.hpp"

class	Server;

class	Client
{
	friend class Server;

public:
	Client();

	bool	isTimeout() const;
	void	updateLastActive();

private:
	int					socketFD;
	sockaddr_storage	address;
	socklen_t			addressLen;

	std::vector<char>	messageBuffer;
	String				message;
	static const size_t	MAX_REQUEST_SIZE;
	Request				request;

	bool				firstDataRecv;
	std::time_t			lastActive;
};

#endif
