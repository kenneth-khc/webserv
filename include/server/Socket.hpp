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
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>

/**
	A class encapsulating a socket. It wraps the syscalls necessary to work with
	sockets, throws exceptions on errors, provides utilities to work with the
	socket addresses, and store string representations of the IP and port.
*/

struct	Socket
{
public:
	Socket();
	~Socket();

	/** Spawns a new socket using the given IP address and port number */
	static Socket	spawn(const String& ip, const String& port);

	/** Wraps an already existing socket with the given fd and address into
		a Socket object  */
	static Socket	wrap(int fd, sockaddr_storage address);

	/** fd of the socket, used by syscalls */
	int				fd;
	/** IP address string */
	String			ip;
	/** port number string */
	String			port;
	unsigned short	portNum;

	/** Binds the newly created socket with the address */
	int		bind() const;
	int		bind(const struct sockaddr*, socklen_t) const;

	/** Marks a socket as listening for incoming connections */
	int		listen(int connectionCount) const;

	/** Accepts an incoming connection, and returns the new Socket used
		for that connection */
	Socket	accept() const;

	/** A functor to invoke to match addresses between two sockets */
	struct IsMatchingAddress
	{
		String			ip;
		unsigned short	port;

		IsMatchingAddress(String ip, unsigned short port):
		ip(ip), port(port)
		{
		}

		bool	operator()(const std::pair<const int, const Socket>& pair) const
		{
			return pair.second.ip == ip && pair.second.port == port;
		}
	};

private:
	/* Internally how the addresses are represented by the socket API,
	 * only used for us to convert into its String representations for ease
	 * of use */
	sockaddr_storage	_address;
	socklen_t			_addressLen;

	Socket(const String&, const String&);
	Socket(int, sockaddr_storage);

	/** Gets addrinfo for a stream socket with given IPv4 address and
		port number, to be used for binding a socket */
	addrinfo*	getAddrInfo(const String&, const String&);
	void		convertAddressToIpAndPort(sockaddr_storage);
};

#endif
