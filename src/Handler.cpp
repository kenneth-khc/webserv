/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:24:45 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/04 18:21:49 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <cstdlib>

#if 0
Request	Server::receiveRequest(int fd) const
{
	Request		request;
	std::string	message;
	ssize_t		bytes;

	while (endOfRequestLineNotFound(message))
	{
		bytes = readIntoBuffer(message, fd);
		std::cout << "bytes: " << bytes << '\n';
		if (bytes < 0)
		{
			throw Response(BadRequest400());
		}
	}
	request.parseRequestLine(message);
	while (endOfHeaderNotFound(message))
	{
		bytes = readIntoBuffer(message, fd);
		if (bytes < 0)
		{
			throw Response(BadRequest400());
		}
	}
	request.parseHeaders(message);

	// if there is a Content-Length field, there is a message body
	std::string	messageBody;
	int	bodyLength = request.find<int>("Content-Length");
	if (bodyLength > 0)
	{
		messageBody += message;
		bodyLength -= messageBody.size();
	}
	while (bodyLength > 0)
	{
		std::string	temp;
		bytes = readIntoBuffer(temp, fd);
		if (bytes < 0)
			break;
		if (bytes > 0)
		{
			messageBody += temp;
			bodyLength -= bytes;
		}
	}
<<<<<<< HEAD
	request.messageBody = messageBody;
	std::cout << ">>> " << request.messageBody << '\n';
=======
<<<<<<< HEAD
	request.parseMessageBody(messageBody);
||||||| 1429036
	request.parseMessageBody(messageBody);
	std::cout << ">>> " << request.messageBody << '\n';
=======
	request.messageBody = messageBody;
	std::cout << ">>> " << request.messageBody << '\n';
>>>>>>> http
>>>>>>> 86ac
	// TODO: is this where we delete the fd? what if
	// the connection is kept alive

	/*epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, NULL);*/
	return request;
}
#endif
