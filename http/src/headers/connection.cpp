/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/10 15:38:02 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <string>
#include "Server.hpp"
#include "Optional.hpp"
#include "connection.hpp"

void	constructConnectionHeader(const Request &request, Response &response) {
	Optional<std::string> connectionOption = request["Connection"];

	for (std::size_t i = 0; i < connectionOption.value.length(); i++)
		connectionOption.value[i] = std::tolower(connectionOption.value[i]);
	if (request.httpVersion >= 1.1 && connectionOption.value.find("close") != std::string::npos)
		response.flags |= Response::CONNECTION_CLOSE;
	else if (connectionOption.value.find("keep-alive") == std::string::npos)
		response.flags |= Response::CONNECTION_CLOSE;

	if (response.flags & Response::CONNECTION_CLOSE)
		response.insert("Connection", "close");
	else {
		std::string			timeout;
		std::stringstream	stream;

		stream << Server::timeoutValue;
		std::getline(stream, timeout, '\0');
		timeout = "timeout=" + timeout;
		response.insert("Connection", "keep-alive");
		response.insert("Keep-Alive", timeout);
	}
}
