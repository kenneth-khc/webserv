/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 16:54:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <deque>
#include <vector>
#include "Time.hpp"
#include "String.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Timer.hpp"
#include "CGI.hpp"

struct	Socket;
struct	Server;

class	Client
{
	friend struct Server;
	friend struct Driver;

public:
	Client();
	Client(Socket*, const Socket*);
	~Client();
	Client(const Client&);
	Client	&operator=(const Client&);

	ssize_t	receiveBytes();
	ssize_t	sendBytes(String &formattedResponse);

	Socket*				socket;
	const Socket*		receivedBy;
	Server*				server;
	String				message;
	Timer*				timer;
	std::vector<CGI*>	cgis;

private:
	std::vector<char>		messageBuffer;
	Request					request;
	std::deque<Request>		requestQueue;
	std::deque<Response>	responseQueue;
	static const size_t		MAX_REQUEST_SIZE;

};

#endif
