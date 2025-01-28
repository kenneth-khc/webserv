/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:20:18 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 05:24:59 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Response.hpp"

Response::Response(void) : httpVersion(1.1) {
	this->headers.insert(std::pair<std::string, std::string>("Server", SERVER_NAME));
}

Response::Response(const Response &obj) :
	httpVersion(obj.httpVersion),
	statusCode(obj.statusCode),
	reasonPhrase(obj.reasonPhrase),
	headers(obj.headers),
	messageBody(obj.messageBody)	
{}

Response&	Response::operator=(const Response& other)
{
	this->httpVersion = other.httpVersion;
	this->statusCode = other.statusCode;
	this->reasonPhrase = other.reasonPhrase;
	this->headers = other.headers;
	this->messageBody = other.messageBody;
	return *this;
}

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
	
	std::getline(stream, str);
	return (str);
}
