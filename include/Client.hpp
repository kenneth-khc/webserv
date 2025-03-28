/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/28 17:18:11 by cteoh            ###   ########.fr       */
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

struct	Socket;
struct	Server;

class	Client
{
	friend struct Server;
	friend struct Driver;

public:
	Client();
	Client(Socket*, const Socket*);
	Client(const Client&);

	ssize_t	receiveBytes();
	ssize_t	sendBytes(String &formattedResponse);

	Socket*				socket;
	const Socket*		receivedBy;
	Server*				server;
	String				message;
private:
	std::vector<char>		messageBuffer;
	static const size_t		MAX_REQUEST_SIZE;
	Request					request;
	std::deque<Request>		requestQueue;
	std::deque<Response>	responseQueue;

	char					timer;
	std::time_t				keepAliveTimeout;
	std::time_t				clientHeaderTimeout;
	std::time_t				clientBodyTimeout;

	enum Timeout {
		KEEP_ALIVE = 0x001,
		CLIENT_HEADER = 0x002,
		CLIENT_BODY = 0x004
	};
};

#endif
