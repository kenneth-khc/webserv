/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/13 12:14:39 by cteoh            ###   ########.fr       */
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
	client(NULL),
	requestLineFound(false),
	headersFound(false),
	hasMessageBody(true),
	messageBodyFound(false),
	chunkSize(0),
	length(0),
	chunkSizeFound(false),
	lastChunk(false)
{}

Request::~Request(void) {}

Request::Request(const Request &obj) :
	Message(obj),
	client(obj.client),
	requestLineFound(obj.requestLineFound),
	method(obj.method),
	requestTarget(obj.requestTarget),
	absolutePath(obj.absolutePath),
	query(obj.query),
	queryPairs(obj.queryPairs),
	headersFound(obj.headersFound),
	cookies(obj.cookies),
	hasMessageBody(obj.hasMessageBody),
	messageBodyFound(obj.messageBodyFound),
	chunkSize(0),
	length(0),
	chunkSizeFound(false),
	lastChunk(false)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	Message::operator=(obj);
	this->client = obj.client;
	this->requestLineFound = obj.requestLineFound;
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	this->absolutePath = obj.absolutePath;
	this->query = obj.query;
	this->queryPairs = obj.queryPairs;
	this->headersFound = obj.headersFound;
	this->cookies = obj.cookies;
	this->hasMessageBody = obj.hasMessageBody;
	this->messageBodyFound = obj.messageBodyFound;
	this->chunkSize = obj.chunkSize;
	this->length = obj.length;
	this->chunkSizeFound = obj.chunkSizeFound;
	this->lastChunk = obj.lastChunk;
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
	this->requestLineFound = true;
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
	this->headersFound = true;
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
		// if (contentLength.value > 100)	// Test-specific condition
		// 	throw ContentTooLarge413();

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

	std::stringstream			stream;
	String						str;
	Optional<String::size_type>	chunkSizeTerminator;
	Optional<String::size_type>	chunkExt;
	Optional<String::size_type>	chunkDataTerminator;

	while (true) {
		if (this->chunkSizeFound == false) {
			chunkSizeTerminator = line.find("\r\n");
			if (chunkSizeTerminator.exists == false)
				return ;

			str = line.substr(0, chunkSizeTerminator.value);

			chunkExt = str.find(";");	// No support for chunk extensions
			if (chunkExt.exists == true)
				str = str.substr(0, chunkExt.value);

			for (String::size_type i = 0; i < str.length(); i++) {
				if (std::isxdigit(str[i]) == 0)
					throw BadRequest400();
			}
			stream << str;
			stream >> std::hex >> this->chunkSize;
			stream.str("");
			stream.clear();

			if (this->chunkSize == 0)
				this->lastChunk = true;

			line.erase(0, chunkSizeTerminator.value + 2);
			this->chunkSizeFound = true;
		}
		if (this->chunkSizeFound == true) {
			String::iterator	it = line.begin();

			while (this->chunkSize > 0) {
				if (it == line.end()) {
					line.erase(line.begin(), it);
					return ;
				}
				this->messageBody += *it;
				this->length++;
				// if (this->length > 100)	// Test-specific condition
				// 	throw ContentTooLarge413();
				it++;
				this->chunkSize--;
			}
			line.erase(line.begin(), it);

			chunkDataTerminator = line.find("\r\n");
			if (chunkDataTerminator.exists == false)
				return ;
			line.erase(0, chunkDataTerminator.value + 2);

			this->chunkSizeFound = false;
			if (this->lastChunk == true)
				break ;
		}
	}
	// No support for chunked trailer section

	this->lastChunk = false;
	this->insert(Request::stringToLower("Content-Length"), this->length);
	this->headers.erase(Request::stringToLower("Transfer-Encoding"));
	this->messageBodyFound = true;
}
