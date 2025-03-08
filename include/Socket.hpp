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

#include <sys/socket.h>

struct	Socket
{
	Socket();
	Socket(int);
	~Socket();

	int	bind();
	int	bind(const struct sockaddr*, socklen_t);
	int	listen(int connectionCount);

	int	fd;
	int	port;
};

#endif
