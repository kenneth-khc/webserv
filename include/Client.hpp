/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:13:38 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/01 13:52:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <string>
#include <vector>
#include "Request.hpp"

class	Server;

class	Client
{
	friend class Server;

public:
	Client();

private:
	int					socketFD;
	sockaddr_storage	address;
	socklen_t			addressLen;

	std::vector<char>	messageBuffer;
	std::string			message;
	static const size_t		MAX_REQUEST_SIZE;
	Request				request;

	bool				requestLineFound;
	bool				headersFound;
	bool				hasBody;
	bool				bodyFound;

};

#endif
