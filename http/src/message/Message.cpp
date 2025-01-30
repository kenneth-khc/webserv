/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:32:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 23:05:49 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

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
	this->headers.insert(std::make_pair(key, value));
}

std::string	Message::operator[](const std::string &key) {
	return (this->headers[key]);
}

template<>
std::string	Message::find<std::string>(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return ("");
	return (it->second);
}

template<>
int	Message::find<int>(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return (std::numeric_limits<int>::min());
	return (std::atoi(it->second.c_str()));
}

template<>
float	Message::find<float>(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return (std::numeric_limits<float>::min());
	return (std::atof(it->second.c_str()));
}
