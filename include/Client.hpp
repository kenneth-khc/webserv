/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2025/03/28 17:18:11 by cteoh            ###   ########.fr       */
=======
/*   Updated: 2025/03/29 21:54:04 by cteoh            ###   ########.fr       */
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
	Socket*				socket;
	const Socket*		receivedBy;
	Server*				server;
	String				message;
=======
	Socket					socket;
	String					message;
	char					timer;

	enum Timeout {
		KEEP_ALIVE = 0x001,
		CLIENT_HEADER = 0x002,
		CLIENT_BODY = 0x004
	};
>>>>>>> Stashed changes
private:
	std::vector<char>		messageBuffer;
	static const size_t		MAX_REQUEST_SIZE;
	Request					request;
	std::deque<Request>		requestQueue;
	std::deque<Response>	responseQueue;

	std::time_t				keepAliveTimeout;
	std::time_t				clientHeaderTimeout;
	std::time_t				clientBodyTimeout;
};

#endif
