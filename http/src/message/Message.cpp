/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:32:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/07 17:45:29 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <stdexcept>
#include "ErrorCode.hpp"
#include "Message.hpp"

const std::string	Message::allowedDuplicateHeaders[NUM_OF_HEADERS] = {
	"accept",
	"accept-encoding",
	"accept-language",
	"accept-ranges",
	"allow",
	"authentication-info",
	"cache-control",
	"connection",
	"content-encoding",
	"content-language",
	"expect",
	"if-match",
	"if-none-match",
	"proxy-authenticate",
	"proxy-aunthetication-info",
	"te",
	"trailer",
	"transfer-encoding",
	"upgrade",
	"vary",
	"via",
	"www-authenticate"
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

std::string	Message::stringToLower(std::string str) {
	for (std::size_t i = 0; i < str.length(); i++)
		str[i] = std::tolower(str[i]);
	return (str);
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
	std::string	lowercaseKey = Message::stringToLower(key);

	try {
		return (this->headers.at(lowercaseKey));
	}
	catch (const std::out_of_range &e) {
		return (Optional<std::string>());
	}
}

const Optional<std::string>	Message::operator[](const std::string &key) const {
	std::string	lowercaseKey = Message::stringToLower(key);

	try {
		return (this->headers.at(lowercaseKey));
	}
	catch (const std::out_of_range &e) {
		return (Optional<std::string>());
	}
}
