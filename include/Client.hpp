/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/23 03:08:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <deque>
#include "Time.hpp"
#include "String.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

class	Client
{
	friend struct Server;
	friend struct Driver;

public:
	Client();

	String	getIPAddr() const;
	String	getPortNum() const;

	ssize_t	receiveBytes();
	ssize_t	sendBytes(Response &response);

	Socket					socket;
	String					message;
private:
	sockaddr_storage		address;
	socklen_t				addressLen;

	std::vector<char>		messageBuffer;
	static const size_t		MAX_REQUEST_SIZE;
	Request					request;
	std::deque<Request>		requestQueue;
	std::deque<Response>	responseQueue;

	bool					keepAlive;
	std::time_t				lastActive;
};

#endif
