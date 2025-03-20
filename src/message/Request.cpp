/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/20 02:59:15 by cteoh            ###   ########.fr       */
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
	chunkSize(0),
	bodyLength(0),
	chunkSizeFound(false),
	lastChunk(false)
{
	this->processStage = Request::REQUEST_LINE;
}

Request::~Request(void) {}

Request::Request(const Request &obj) :
	Message(obj),
	method(obj.method),
	requestTarget(obj.requestTarget),
	path(obj.path),
	query(obj.query),
	queryPairs(obj.queryPairs),
	cookies(obj.cookies),
	chunkSize(0),
	bodyLength(0),
	chunkSizeFound(false),
	lastChunk(false)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	Message::operator=(obj);
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	this->path = obj.path;
	this->query = obj.query;
	this->queryPairs = obj.queryPairs;
	this->cookies = obj.cookies;
	this->chunkSize = obj.chunkSize;
	this->bodyLength = obj.bodyLength;
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
	if (line.find("\r\n").exists == false)
		return ;

	String						str;
	Optional<String::size_type>	terminatorPos = line.find("\r\n");

	if (terminatorPos.exists == false)
		throw BadRequest400();
	str = line.substr(0, terminatorPos.value);
	extractRequestLineComponents(*this, str);
	line = line.substr(terminatorPos.value + 2);
	this->processStage = Request::HEADERS;
}

void	Request::parseHeaders(String &line) {
	if (line.find("\r\n\r\n").exists == false)
		return ;

	String						str;
	Optional<String::size_type>	headerLineTerminator;
	Optional<String::size_type>	headerLineStart(0);
	Optional<String::size_type>	headerSectionTerminator = line.find("\r\n\r\n");

	if (headerSectionTerminator.exists == false)
		throw BadRequest400();

	while (true) {
		headerLineTerminator = line.find("\r\n", headerLineStart.value);
		str = line.substr(headerLineStart.value, headerLineTerminator.value - headerLineStart.value);

		extractFieldLineComponents(*this, str);

		headerLineStart.value = headerLineTerminator.value + 2;
		if (headerLineStart.value >= headerSectionTerminator.value)
			break ;
	}
	if (line[headerLineStart.value + 2] == '\0')
		line = "";
	else
		line = line.substr(headerLineStart.value + 2);
	this->processStage = Request::HEAD_DONE;
}

void	Request::parseCookieHeader(void) {
	Optional<String>	cookieHeader = (*this)["Cookie"];

	if (cookieHeader.exists == false)
		return ;

	isCookieString(this->cookies, cookieHeader.value);
}

void	Request::checkIfBodyExists(void) {
	Optional<String>	contentLength = (*this)["Content-Length"];
	Optional<String>	transferEncoding = (*this)["Transfer-Encoding"];

	if (transferEncoding.exists == true)
	{
		if (contentLength.exists == true)
		{
			this->headers.erase("content-length");
		}
		transferEncoding.value = transferEncoding.value.lower();
		if (transferEncoding.value.find("chunked").exists == false)
		{
			throw BadRequest400();
		}
		this->processStage = Request::READY | Request::MESSAGE_BODY;
	}
	else if (contentLength.exists == true)
	{
		if (isContentLengthHeader(contentLength.value) == false)
		{
			throw BadRequest400();
		}
		this->bodyLength = this->find< Optional<String::size_type> >("Content-Length").value;
		this->processStage = Request::READY | Request::MESSAGE_BODY;
	}
	else
		this->processStage = Request::READY | Request::DONE;
}

void	Request::parseMessageBody(String &line) {
	Optional<String>	contentLength = (*this)["Content-Length"];

	if (contentLength.exists == true) {
		String::iterator	it = line.begin();

		while (this->messageBody.length() < this->bodyLength && it != line.end()) {
			this->messageBody += *it;
			it++;
		}
		line.erase(line.begin(), it);
		if (this->messageBody.length() == this->bodyLength) {
			this->processStage &= ~Request::MESSAGE_BODY;
			if (this->processStage == 0)
				this->processStage = Request::DONE;
		}
		return ;
	}

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

			std::stringstream(str) >> std::hex >> this->chunkSize;
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
				this->bodyLength++;
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
	this->insert("content-length", this->bodyLength);
	this->headers.erase("transfer-encoding");
	this->processStage &= ~Request::MESSAGE_BODY;
	if (this->processStage == 0)
		this->processStage = Request::DONE;
}
