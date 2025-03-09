/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:33:01 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/08 19:55:47 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "String.hpp"
#include "Client.hpp"
#include <sys/socket.h>
#include <netdb.h>

struct	Socket
{
	Socket();
	Socket(String, int);
	Socket(sockaddr_storage);
	Socket(int);
	~Socket();

	int		bind();
	int		bind(const struct sockaddr*, socklen_t);
	int		listen(int connectionCount);
	Client	acceptNewConnection() const;

	String	getIPAddr() const;
	String	getPortNum() const;

	int				fd;
	addrinfo*		info;
	String			ip;
	unsigned short	portNum;
	String			port;

private:
	void	fillAddress(sockaddr_storage);
};

#endif
