/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 15:46:20 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "requestLine.hpp"
#include "requestFields.hpp"
#include "ErrorCode.hpp"
#include "Request.hpp"

const String	Request::methods[NUM_OF_METHODS] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"OPTIONS",
	"DELETE"
};

const float	Request::supportedVersions[NUM_OF_VERSIONS] = { 1.1 };

Request::Request(void) :
	Message(),
	method(0),
	requestTarget(""),
	flags(0),
	requestLineFound(false),
	headersFound(false)
{}

Request::~Request(void) {}

Request::Request(const Request &obj) :
	Message(obj),
	method(obj.method),
	requestTarget(obj.requestTarget),
	flags(obj.flags),
	requestLineFound(obj.requestLineFound),
	headersFound(obj.headersFound)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	Message::operator=(obj);
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	this->flags = obj.flags;
	this->requestLineFound = obj.requestLineFound;
	this->headersFound = obj.headersFound;
	return *this;
}

bool	Request::isValidMethod(const String &method) {
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

void	Request::parseRequestLine(String &line) {
	String	str;
	std::size_t	terminatorPos = 0;

	terminatorPos = line.find("\r\n", 0);
	try {
		if (terminatorPos == String::npos)
			throw BadRequest400();
		str = line.substr(0, terminatorPos);
		extractRequestLineComponents(str, *this);
	}
	catch (const ErrorCode &error) {
		throw Response(error);
	}
	line = line.substr(terminatorPos + 2);
}

void	Request::parseHeaders(String &line) {
	String		str;
	std::size_t	headerLineTerminator = 0;
	std::size_t	headerLineStart = 0;
	std::size_t	headerSectionTerminator = line.find("\r\n\r\n");

	if (headerSectionTerminator == String::npos)
		throw (Response(BadRequest400()));

	while (true) {
		headerLineTerminator = line.find("\r\n", headerLineStart);
		str = line.substr(headerLineStart, headerLineTerminator - headerLineStart);

		try {
			extractFieldLineComponents(str, *this);
		}
		catch (const ErrorCode &error) {
			throw Response(error);
		}

		headerLineStart = headerLineTerminator + 2;
		if (headerLineStart >= headerSectionTerminator)
			break ;
	}
	if (line[headerLineStart + 2] == '\0') {
		line = "";
		return ;
	}
	line = line.substr(headerLineStart + 2);
}
