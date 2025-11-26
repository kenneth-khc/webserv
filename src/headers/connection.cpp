/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/21 05:20:08 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <sstream>
#include "String.hpp"
#include "Server.hpp"
#include "Optional.hpp"
#include "connection.hpp"

/*
	Appends the connection header depending on whether request specifies to
	close connection and the HTTP version.
*/
void	constructConnectionHeader(const Request &request, Response &response) {
	Optional<String> connectionOption = request["Connection"];

	connectionOption.value = connectionOption.value.lower();
	if (connectionOption.value.find("close").exists == true)
		response.closeConnection = true;
	else if (request.httpVersion != "1.0")
		response.closeConnection = false;
	else if (request.httpVersion == "1.0" &&
		connectionOption.value.find("keep-alive").exists == true)
		   response.closeConnection = false;
	else
		response.closeConnection = true;

	if (response.closeConnection == true)
		response.insert("Connection", "close");
	else {
		std::stringstream	stream;

		stream << Server::keepAliveTimeoutDuration;
		response.insert("Connection", "keep-alive");
		response.insert("Keep-Alive", "timeout=" + stream.str());
	}
}
