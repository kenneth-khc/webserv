/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 02:17:46 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "String.hpp"
#include "Server.hpp"
#include "Optional.hpp"
#include "connection.hpp"

void	constructConnectionHeader(const Request &request, Response &response) {
	Optional<String> connectionOption = request["Connection"];

	for (std::size_t i = 0; i < connectionOption.value.length(); i++)
		connectionOption.value[i] = std::tolower(connectionOption.value[i]);
	if (request.httpVersion >= 1.1 && connectionOption.value.find("close") != String::npos)
		response.flags |= Response::CONNECTION_CLOSE;
	else if (connectionOption.value.find("keep-alive") == String::npos)
		response.flags |= Response::CONNECTION_CLOSE;

	if (response.flags & Response::CONNECTION_CLOSE)
		response.insert("Connection", "close");
	else {
		String				timeout;
		std::stringstream	stream;

		stream << Server::timeoutValue;
		String::getline(stream, timeout, '\0');
		timeout = "timeout=" + timeout;
		response.insert("Connection", "keep-alive");
		response.insert("Keep-Alive", timeout);
	}
}
