/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:32:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/21 19:15:50 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
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

Message::Message(void) :
	processStage(0),
	httpVersion(0.0)
{}

Message::~Message(void) {}

Message::Message(const Message &obj) :
	processStage(obj.processStage),
	httpVersion(obj.httpVersion),
	headers(obj.headers),
	messageBody(obj.messageBody)
{}

Message	&Message::operator=(const Message &obj) {
	if (this == &obj)
		return (*this);
	this->processStage = obj.processStage;
	this->httpVersion = obj.httpVersion;
	this->headers = obj.headers;
	this->messageBody = obj.messageBody;
	return (*this);
}

void	Message::insert(const String &key, const String &value) {
	this->headers.insert(std::make_pair(key, value));
}

void	Message::insert(const String &key, const String::size_type &value) {
	std::stringstream	stream;

	stream << value;
	this->headers.insert(std::make_pair(key, stream.str()));
}

Optional<String>	Message::operator[](const String &key) {
	std::multimap<String, String>::iterator	it = this->headers.find(key);

	if (it == this->headers.end())
		return (Optional<String>());
	else
		return (it->second);
}

const Optional<String>	Message::operator[](const String &key) const {
	std::multimap<String, String>::const_iterator	it = this->headers.find(key);

	if (it == this->headers.end())
		return (Optional<String>());
	else
		return (it->second);
}
