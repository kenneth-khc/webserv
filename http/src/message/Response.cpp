/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:20:18 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 21:23:33 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Response.hpp"

Response::Response(void) : httpVersion(1.1) {
	this->headers.insert(std::make_pair("Server", SERVER_NAME));
}

Response::~Response(void) {}

Response::Response(const Response &obj) :
	httpVersion(obj.httpVersion),
	statusCode(obj.statusCode),
	reasonPhrase(obj.reasonPhrase),
	headers(obj.headers),
	messageBody(obj.messageBody)
{}

Response&	Response::operator=(const Response& other)
{
	if (this == &other)
		return (*this);
	this->httpVersion = other.httpVersion;
	this->statusCode = other.statusCode;
	this->reasonPhrase = other.reasonPhrase;
	this->headers = other.headers;
	this->messageBody = other.messageBody;
	return *this;
}

//	Turns the information stored in the Response instance into a complete
//	HTTP response message
const std::string	Response::toString(void) const {
	std::stringstream	stream;
	std::string			str;
	
	stream << "HTTP/" << this->httpVersion << ' ';
	stream << this->statusCode << ' ';
	stream << this->reasonPhrase << "\r\n";

	if (this->headers.size() != 0) {
		for (std::map<std::string, std::string>::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
			stream << it->first << ": " << it->second << "\r\n";
	}
	stream << "\r\n";

	if (this->messageBody.length() != 0)
		stream << this->messageBody;
	
	std::getline(stream, str, '\0');
	return (str);
}
