/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/19 22:08:35 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <typeinfo>
#include <sstream>
#include "ErrorCode.hpp"
#include "Request.hpp"
#include "RequestLineState.hpp"

const String	Request::methods[NUM_OF_METHODS] = {
	"GET",
	"POST",
	"DELETE"
};

const String	Request::supportedVersions[NUM_OF_VERSIONS] = { "1.0", "1.1" };

Request::Request(void) :
	Message(),
	method(),
	requestTarget(),
	path(),
	resolvedPath(),
	query(),
	decodedQuery(),
	queryPairs(),
	cookies(),
	state(0)
{}

Request::~Request(void) {
	if (this->state != 0)
		delete this->state;
}

Request::Request(const Request &obj) :
	Message(obj),
	method(obj.method),
	requestTarget(obj.requestTarget),
	path(obj.path),
	resolvedPath(obj.resolvedPath),
	query(obj.query),
	queryPairs(obj.queryPairs),
	cookies(obj.cookies),
	state(obj.state)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	Message::operator=(obj);
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	this->path = obj.path;
	this->resolvedPath = obj.resolvedPath;
	this->query = obj.query;
	this->queryPairs = obj.queryPairs;
	this->cookies = obj.cookies;
	this->state = obj.state;
	return *this;
}

void	Request::insert(const String &key, const String &value) {
	std::multimap<String, String>::iterator	it = this->headers.begin();
	String									lowercase = key.lower();

	it = this->headers.find(lowercase);
	if (it == this->headers.end())
		this->headers.insert(std::make_pair(lowercase, value));
	else {
		for (int i = 0; i < NUM_OF_HEADERS; i++) {
			if (allowedDuplicateHeaders[i] == lowercase) {
				it->second += ", " + value;
				return ;
			}
		}
		throw BadRequest400();
	}
}

void	Request::insert(const String &key, const String::size_type &value) {
	std::multimap<String, String>::iterator	it = this->headers.begin();
	String									lowercase = key.lower();
	std::stringstream						stream;

	it = this->headers.find(lowercase);
	stream << value;
	if (it == this->headers.end())
		this->headers.insert(std::make_pair(lowercase, stream.str()));
	else {
		for (int i = 0; i < NUM_OF_HEADERS; i++) {
			if (allowedDuplicateHeaders[i] == lowercase) {
				it->second += ", " + stream.str();
				return ;
			}
		}
		throw BadRequest400();
	}
}

Optional<String>	Request::operator[](const String &key) {
	return (Message::operator[](key.lower()));
}

const Optional<String>	Request::operator[](const String &key) const {
	return (Message::operator[](key.lower()));
}

void	Request::erase(const String &key) {
	this->headers.erase(key.lower());
}

RequestState	*Request::processState(Client &client) {
	if (this->state == 0)
		this->state = new RequestLineState();

	RequestState	*newState = this->state->process(*this, client);

	if (newState->getState() == RequestState::HEAD_DONE)
	{
		Logger::logRequest(*this, client);
	}
	if (this->state != newState) {
		delete this->state;
		this->state = newState;
	}
	return (this->state);
}

void	Request::parseCookieHeader(void) {
	Optional<String>	cookieHeader = (*this)["Cookie"];

	if (cookieHeader.exists == false)
		return ;

	isCookieString(this->cookies, cookieHeader.value);
}

bool	Request::isValidMethod(void) {
	for (int i = 0; i < NUM_OF_METHODS; i++) {
		if (this->method == methods[i])
			return (true);
	}
	return (false);
}

bool	Request::isSupportedVersion(void) {
	for (int i = 0; i < NUM_OF_HEADERS; i++) {
		if (this->httpVersion == supportedVersions[i])
			return (true);
		i++;
	}
	return (false);
}
