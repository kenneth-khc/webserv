/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:32:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/16 23:23:41 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <stdexcept>
#include "ErrorCode.hpp"
#include "Message.hpp"

const String	Message::allowedDuplicateHeaders[NUM_OF_HEADERS] = {
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

String	Message::stringToLower(String str) {
	for (String::size_type i = 0; i < str.length(); i++)
		str[i] = std::tolower(str[i]);
	return (str);
}

void	Message::insert(const String &key, const String &value) {
	std::multimap<String, String>::iterator	it = this->headers.begin();

	it = this->headers.find(key);
	if (it == this->headers.end() || key == "Set-Cookie") {
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

void	Message::insert(const String &key, const int &value) {
	std::multimap<String, String>::iterator	it = this->headers.begin();
	std::stringstream	stream;
	String				str;

	it = this->headers.find(key);
	stream << value;
	String::getline(stream, str, '\0');
	if (it == this->headers.end() || key == "Set-Cookie") {
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

Optional<String>	Message::operator[](const String &key) {
	String									lowercaseKey = Message::stringToLower(key);
	std::multimap<String, String>::iterator	it = this->headers.find(lowercaseKey);

	if (it == this->headers.end())
		return (Optional<String>());
	else
		return (it->second);
}

const Optional<String>	Message::operator[](const String &key) const {
	String											lowercaseKey = Message::stringToLower(key);
	std::multimap<String, String>::const_iterator	it = this->headers.find(lowercaseKey);

	if (it == this->headers.end())
		return (Optional<String>());
	else
		return (it->second);
}
