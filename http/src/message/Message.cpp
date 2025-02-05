/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:32:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 17:21:31 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <stdexcept>
#include "ErrorCode.hpp"
#include "Message.hpp"

const std::string	Message::allowedDuplicateHeaders[NUM_OF_HEADERS] = {
	"Accept",
	"Accept-Encoding",
	"Accept-Language",
	"Accept-Ranges",
	"Allow",
	"Authentication-Info",
	"Cache-Control",
	"Connection",
	"Content-Encoding",
	"Content-Language",
	"Expect",
	"If-Match",
	"If-None-Match",
	"Proxy-Authenticate",
	"Proxy-Aunthetication-Info",
	"TE",
	"Trailer",
	"Transfer-Encoding",
	"Upgrade",
	"Vary",
	"Via",
	"WWW-Authenticate"
};

Message::Message(void) {}

Message::~Message(void) {}

Message::Message(const Message &obj) :
	httpVersion(obj.httpVersion),
	headers(obj.headers),
	messageBody(obj.messageBody)
{}

Message	&Message::operator=(const Message &obj) {
	if (this == &obj)
		return (*this);
	this->httpVersion = obj.httpVersion;
	this->headers = obj.headers;
	this->messageBody = obj.messageBody;
	return (*this);
}

void	Message::insert(const std::string &key, const std::string &value) {
	std::map<std::string, std::string>::iterator	it = this->headers.begin();

	it = this->headers.find(key);
	if (it == this->headers.end()) {
		this->headers.insert(std::make_pair(key, value));
	}
	else {
		for (int i = 0; i < NUM_OF_HEADERS; i++) {
			if (allowedDuplicateHeaders[i] == key) {
				it->second += ", " + value;
				return ;
			}
		}
		throw BadRequest400();
	}
}

void	Message::insert(const std::string &key, const int &value) {
	std::map<std::string, std::string>::iterator	it = this->headers.begin();
	std::stringstream								stream;
	std::string										str;

	it = this->headers.find(key);
	stream << value;
	std::getline(stream, str, '\0');
	if (it == this->headers.end()) {
		this->headers.insert(std::make_pair(key, str));
	}
	else {
		for (int i = 0; i < NUM_OF_HEADERS; i++) {
			if (allowedDuplicateHeaders[i] == key) {
				it->second += ", " + str;
				return ;
			}
		}
		throw BadRequest400();
	}
}

Optional<std::string>	Message::operator[](const std::string &key) {
	try {
		return (this->headers.at(key));
	}
	catch (const std::out_of_range &e) {
		return (Optional<std::string>());
	}
}

const Optional<std::string>	Message::operator[](const std::string &key) const {
	try {
		return (this->headers.at(key));
	}
	catch (const std::out_of_range &e) {
		return (Optional<std::string>());
	}
}