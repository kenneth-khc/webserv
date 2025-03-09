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
public:
	Socket();
	~Socket();
	static Socket	spawn(const String&, const String&);
	static Socket	wrap(int, sockaddr_storage);

	int				fd;
	String			ip;
	String			port;
	unsigned short	portNum;

	int		bind() const;
	int		bind(const struct sockaddr*, socklen_t) const;
	int		listen(int connectionCount) const;
	Socket	accept() const;

private:
	/* Internally how the addresses are represented by the socket API,
	 * only used for us to convert into its String representations for ease
	 * of use */
	sockaddr_storage	_address;
	socklen_t			_addressLen;

	Socket(const String&, const String&);
	Socket(int, sockaddr_storage);

	addrinfo*	getAddrInfo(const String&, const String&);
	void		fillAddress(sockaddr_storage);
};

/* A functor to pass into std::find_if to match port numbers because
 * lambdas don't exist in C++98 ..... */
struct	IsMatchingPort
{
	unsigned short	port;

	IsMatchingPort(unsigned short port): port(port) {}

	bool	operator()(const std::pair<const int, const Socket>& pair) const
	{
		return pair.second.port == port;
	}
};

#endif
