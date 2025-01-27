/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:41:48 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 21:19:55 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "requestLine.hpp"
#include "fields.hpp"
#include "messageBody.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ErrorCode.hpp"

bool	parseMessage(const std::string &line) {
	Request		request;
	Response	response;

	std::string	str;
	std::size_t	terminatorPos = 0;

	terminatorPos = line.find("\r\n", 0);
	if (terminatorPos == std::string::npos)
		return (false);
	str = line.substr(0, terminatorPos);
	try {
		if (isRequestLine(str, request) == false)
			return (false);
	}
	catch (const ErrorCode &error) {
		response.httpVersion = error.httpVersion;
		response.statusCode = error.statusCode;
		response.reasonPhrase = error.reasonPhrase;
		return (false);
	}

	std::size_t	prevTerminatorPos = terminatorPos;

	terminatorPos = line.find("\r\n", prevTerminatorPos + 2);
	if (terminatorPos == std::string::npos)
		return (false);
	while (terminatorPos != prevTerminatorPos + 2) {
		str = line.substr(prevTerminatorPos + 2, terminatorPos);
		try {
			if (isRequestLine(str, request) == false)
				return (false);
		}
		catch (const ErrorCode &error) {
			response.httpVersion = error.httpVersion;
			response.statusCode = error.statusCode;
			response.reasonPhrase = error.reasonPhrase;
			return (false);
		}
		prevTerminatorPos = terminatorPos;
		terminatorPos = line.find("\r\n", prevTerminatorPos + 2);
		if (terminatorPos == std::string::npos)
			return (false);
	}

	if (line[terminatorPos + 2] == '\0')
		return (true);
	str = line.substr(terminatorPos + 2);
	if (isMessageBody(str) == false)
		return (false);
	return (true);
}
