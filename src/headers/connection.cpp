/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/19 23:47:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "String.hpp"
#include "Server.hpp"
#include "Optional.hpp"
#include "connection.hpp"

void	constructConnectionHeader(const Request &request, Response &response) {
	Optional<String> connectionOption = request["Connection"];

	connectionOption.value = connectionOption.value.lower();
	if (request.httpVersion >= 1.1 && connectionOption.value.find("close").exists == true)
		response.closeConnection = true;
	else if (connectionOption.value.find("keep-alive").exists == false)
		response.closeConnection = true;

	if (response.closeConnection == true)
		response.insert("Connection", "close");
	else {
		std::stringstream	stream;

		stream << Server::clientTimeoutValue;
		response.insert("Connection", "keep-alive");
		response.insert("Keep-Alive", "timeout=" + stream.str());
	}
}
