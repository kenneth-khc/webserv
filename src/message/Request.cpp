/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/26 23:40:06 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Optional.hpp"
#include "ErrorCode.hpp"
#include "requestLine.hpp"
#include "requestFields.hpp"
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
	headersFound(obj.headersFound),
	cookies(obj.cookies)
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
	this->cookies = obj.cookies;
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
	String						str;
	Optional<String::size_type>	terminatorPos = line.find("\r\n");

	if (terminatorPos.exists == false)
		throw BadRequest400();
	str = line.substr(0, terminatorPos.value);
	extractRequestLineComponents(str, *this);
	line = line.substr(terminatorPos.value + 2);
}

void	Request::parseHeaders(String &line) {
	String						str;
	Optional<String::size_type>	headerLineTerminator = 0;
	Optional<String::size_type>	headerLineStart = 0;
	Optional<String::size_type>	headerSectionTerminator = line.find("\r\n\r\n");

	if (headerSectionTerminator.exists == false)
		throw BadRequest400();

	while (true) {
		headerLineTerminator = line.find("\r\n", headerLineStart.value);
		str = line.substr(headerLineStart.value, headerLineTerminator.value - headerLineStart.value);

		extractFieldLineComponents(str, *this);

		headerLineStart.value = headerLineTerminator.value + 2;
		if (headerLineStart.value >= headerSectionTerminator.value)
			break ;
	}
	if (line[headerLineStart.value + 2] == '\0') {
		line = "";
		return ;
	}
	line = line.substr(headerLineStart.value + 2);
}

void	Request::parseCookieHeader(void) {
	Optional<String>	cookieHeader = Message::operator[]("Cookie");

	if (cookieHeader.exists == false)
		return ;

	isCookieString(cookieHeader.value, this->cookies);
}
