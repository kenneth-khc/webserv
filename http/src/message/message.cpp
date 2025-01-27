/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:41:48 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 04:57:59 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "requestLine.hpp"
#include "fields.hpp"
#include "messageBody.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ErrorCode.hpp"

Request	constructRequest(const std::string &line) {
	Request		request;
	std::string	str;
	std::size_t	terminatorPos = 0;

	terminatorPos = line.find("\r\n", 0);
	try {
		if (terminatorPos == std::string::npos)
			throw BadRequest400();
		str = line.substr(0, terminatorPos);
		getRequestLine(str, request);
	}
	catch (const ErrorCode &error) {
		throw Response(error);
	}

	std::size_t	prevTerminatorPos = terminatorPos;

	terminatorPos = line.find("\r\n", prevTerminatorPos + 2);
	try {
		if (terminatorPos == std::string::npos)
			throw BadRequest400();
	}
	catch (const ErrorCode &error) {
		throw Response(error);
	}


	while (terminatorPos != prevTerminatorPos + 2) {
		str = line.substr(prevTerminatorPos + 2, terminatorPos);

		try {
			getFieldLine(str, request);
		}
		catch (const ErrorCode &error) {
			throw Response(error);
		}

		prevTerminatorPos = terminatorPos;
		terminatorPos = line.find("\r\n", prevTerminatorPos + 2);

		try {
			if (terminatorPos == std::string::npos)
				throw BadRequest400();
		}
		catch (const ErrorCode &error) {
			throw Response(error);
		}
	}

	if (line[terminatorPos + 2] == '\0')
		return (request);
	str = line.substr(terminatorPos + 2);
	try {
		if (isMessageBody(str) == false)
			throw BadRequest400();
		else
			request.messageBody = str;
	}
	catch (const ErrorCode &error) {
		throw Response(error);
	}
	return (request);
}
