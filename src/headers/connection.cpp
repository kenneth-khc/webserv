/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/21 17:12:43 by cteoh            ###   ########.fr       */
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
	if (request.httpVersion >= 1.1 && connectionOption.value.find("close").exists == false)
		response.closeConnection = false;
	else if (request.httpVersion < 1.1 && connectionOption.value.find("keep-alive").exists == true)
		response.closeConnection = false;

	if (response.closeConnection == true)
		response.insert("Connection", "close");
	else {
		std::stringstream	stream;

		stream << Server::keepAliveTimeout;
		response.insert("Connection", "keep-alive");
		response.insert("Keep-Alive", "timeout=" + stream.str());
	}
}
