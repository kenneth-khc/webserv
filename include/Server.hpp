/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:04:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/19 17:47:12 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/epoll.h>

class	Server
{
public:
	Server();
	~Server();

	void		startListening();
	void		initEpoll();
	void		epollWait();
	void		acceptNewClient();
	void		processReadyEvents();
	std::string	receiveRequest(int fd) const;

private:
	int				epollFD; // fd of the epoll instance

	std::string		hostName;
	int				portNum;
	int				listenerSocketFD;
	int				numClients; // how many total clients that came through

	int				maxEvents; // how many events to handle each poll
	epoll_event*	readyEvents; // the array of ready events
	int				numReadyEvents; // how many events are ready after a poll

	/* Disallow copying a server */
	Server(Server const&);
	Server&	operator=(Server const&);
};

#endif
