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

struct	Socket;
struct	Server;

class	Client
{
	friend struct Server;
	friend struct Driver;

public:
	Client();
	Client(Socket*, const Socket*);

	bool	endOfRequestLineFound() const;
	bool	endOfHeaderFound() const;

	bool	isTimeout() const;
	void	updateLastActive();

	Socket*				socket;
	const Socket*		receivedBy;
	Server*				server;

private:
	std::vector<char>	messageBuffer;
	String				message;
	static const size_t	MAX_REQUEST_SIZE;
	Request				request;

	bool				firstDataRecv;
	std::time_t			lastActive;
};

#endif
