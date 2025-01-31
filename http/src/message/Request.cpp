/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/01 03:05:05 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "requestLine.hpp"
#include "fields.hpp"
#include "ErrorCode.hpp"
#include "Request.hpp"

const std::string	Request::methods[NUM_OF_METHODS] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"OPTIONS",
	"DELETE"
};

const float	Request::supportedVersions[NUM_OF_VERSIONS] = { 1.1 };

Request::Request(void) : Message() {}

Request::~Request(void) {}

Request::Request(const Request &obj) : 
	Message(obj),
	method(obj.method),
	requestTarget(obj.requestTarget)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	Message::operator=(obj);
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	return *this;
}

bool	Request::isValidMethod(const std::string &method) {
	for (int i = 0; i < NUM_OF_METHODS; i++) {
		if (method == methods[i]) {
			this->method = i;
			return (true);
		}
	}
	return (false);
}

bool	Request::isSupportedVersion(const float &version) {
	for (int i = 0; i < NUM_OF_VERSIONS; i++) {
		if (version > supportedVersions[i])
			return (false);
	}
	return (true);
}

void	Request::parseRequestLine(std::string &line) {
	std::string	str;
	std::size_t	terminatorPos = 0;

	terminatorPos = line.find("\r\n", 0);
	try {
		if (terminatorPos == std::string::npos)
			throw BadRequest400();
		str = line.substr(0, terminatorPos);
		extractRequestLineComponents(str, *this);
	}
	catch (const ErrorCode &error) {
		throw Response(error);
	}
	line = line.substr(terminatorPos + 2);
}

void	Request::parseHeaders(std::string &line) {
	std::string	str;
	std::size_t	headerLineTerminator = 0;
	std::size_t	headerSectionTerminator = line.find("\r\n\r\n");

	if (headerSectionTerminator == std::string::npos)
		throw (Response(BadRequest400()));

	while (true) {
		headerLineTerminator = line.find("\r\n", headerLineTerminator);
		str = line.substr(0, headerLineTerminator);

		try {
			extractFieldLineComponents(str, *this);
		}
		catch (const ErrorCode &error) {
			throw Response(error);
		}

		headerLineTerminator += 2;
		if (headerLineTerminator >= headerSectionTerminator)
			break ;
	}
	if (line[headerLineTerminator + 4] == '\0') {
		line = "";
		return ;
	}
	line = line.substr(headerLineTerminator + 4);
}
