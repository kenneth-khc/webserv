/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/08 19:57:34 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <vector>
#include "Time.hpp"
#include "String.hpp"
#include "Request.hpp"
#include "Socket.hpp"

class	Client
{
	friend struct Server;
	friend struct Driver;

public:
	Client();

	String	getIPAddr() const;
	String	getPortNum() const;

	bool	endOfRequestLineFound() const;
	bool	endOfHeaderFound() const;

	bool	isTimeout() const;
	void	updateLastActive();

	Socket				socket;
private:
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
