/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/06 07:57:36 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ErrorCode.hpp"
#include "requestLine.hpp"
#include "requestFields.hpp"
#include "contentLength.hpp"
#include "Request.hpp"

const String	Request::methods[NUM_OF_METHODS] = {
	"GET",
	"HEAD",
	"POST",
	"DELETE"
};

const float	Request::supportedVersions[NUM_OF_VERSIONS] = { 1.1 };

Request::Request(void) :
	Message(),
	requestLineFound(false),
	headersFound(false),
	messageBodyFound(false)
{}

Request::~Request(void) {}

Request::Request(const Request &obj) :
	Message(obj),
	method(obj.method),
	requestTarget(obj.requestTarget),
	absolutePath(obj.absolutePath),
	query(obj.query),
	queryPairs(obj.queryPairs),
	requestLineFound(obj.requestLineFound),
	headersFound(obj.headersFound),
	messageBodyFound(obj.messageBodyFound),
	cookies(obj.cookies)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	Message::operator=(obj);
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	this->absolutePath = obj.absolutePath;
	this->query = obj.query;
	this->queryPairs = obj.queryPairs;
	this->requestLineFound = obj.requestLineFound;
	this->headersFound = obj.headersFound;
	this->messageBodyFound = obj.messageBodyFound;
	this->cookies = obj.cookies;
	return *this;
}

bool	Request::isValidMethod(const String &method) {
	for (int i = 0; i < NUM_OF_METHODS; i++) {
		if (method == methods[i]) {
			this->method = methods[i];
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
	Optional<String::size_type>	headerLineTerminator;
	Optional<String::size_type>	headerLineStart(0);
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
	Optional<String>	cookieHeader = (*this)["Cookie"];

	if (cookieHeader.exists == false)
		return ;

	isCookieString(cookieHeader.value, this->cookies);
}

void	Request::parseMessageBody(String &line) {
	Optional<String::size_type>	contentLength = this->find< Optional<String::size_type> >("Content-Length");

	if (contentLength.exists == true) {
		String::iterator	it = line.begin();

		while (this->messageBody.length() < contentLength.value && it != line.end()) {
			this->messageBody += *it;
			it++;
		}
		line.erase(line.begin(), it);
		if (this->messageBody.length() == contentLength.value)
			this->messageBodyFound = true;
		return ;
	}

	Optional<String::size_type>	chunkedBodyTerminator = line.find("\r\n\r\n");

	if (chunkedBodyTerminator.exists == false)
		return ;

	String::size_type			length = 0;
	std::stringstream			stream;
	Optional<String::size_type>	chunkExt;
	Optional<String::size_type>	chunkLineStart(0);
	Optional<String::size_type>	chunkLineTerminator;
	String						str;
	String::size_type			chunkSize;

	while (true) {
		chunkLineTerminator = line.find("\r\n", chunkLineStart.value);
		str = line.substr(chunkLineStart.value, chunkLineTerminator.value - chunkLineStart.value);

		chunkExt = str.find(";", chunkLineStart.value);	// No support for chunk extensions
		if (chunkExt.exists == true)
			str = str.substr(0, chunkExt.value);

		for (String::size_type i = 0; i < str.length(); i++) {
			if (std::isxdigit(str[i]) == 0)
				throw BadRequest400();
		}
		stream << str;
		stream >> std::hex >> chunkSize;
		stream.str("");
		stream.clear();

		if (chunkSize == 0)
			break ;

		chunkLineStart.value = chunkLineTerminator.value + 2;
		if (chunkLineStart.value > chunkedBodyTerminator.value && chunkSize != 0)
			throw BadRequest400();

		chunkLineTerminator.value = chunkLineStart.value + chunkSize;
		str = line.substr(chunkLineStart.value, chunkLineTerminator.value - chunkLineStart.value);
		this->messageBody += str;
		length += chunkSize;

		chunkLineStart.value = chunkLineTerminator.value + 2;
		if (chunkLineStart.value > chunkedBodyTerminator.value && chunkSize != 0)
			throw BadRequest400();
	}
	// No support for chunked trailer section

	this->insert(Request::stringToLower("Content-Length"), length);
	this->headers.erase(Request::stringToLower("Transfer-Encoding"));
	this->messageBodyFound = true;
	if (line[chunkLineStart.value + 4] == '\0')
		line = "";
	else
		line = line.substr(chunkLineStart.value + 4);
}
