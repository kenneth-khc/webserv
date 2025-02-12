/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:20:18 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 15:58:17 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <sstream>
#include "Response.hpp"

Response::Response(void) :
	Message(),
	flags(0)
{
	this->httpVersion = 1.1;
	this->insert("Server", SERVER_NAME);
}

Response::~Response(void) {}

Response::Response(const Response &obj) :
	Message(obj),
	statusCode(obj.statusCode),
	reasonPhrase(obj.reasonPhrase),
	flags(obj.flags)
{}

Response&	Response::operator=(const Response& other)
{
	if (this == &other)
		return (*this);
	Message::operator=(other);
	this->statusCode = other.statusCode;
	this->reasonPhrase = other.reasonPhrase;
	this->flags = other.flags;
	return *this;
}

//	Turns the information stored in the Response instance into a complete
//	HTTP response message
const String	Response::toString(void) const {
	std::stringstream	stream;
	String				str;

	stream << "HTTP/" << this->httpVersion << ' ';
	stream << this->statusCode << ' ';
	stream << this->reasonPhrase << "\r\n";

	if (this->headers.size() != 0) {
		for (std::map<String, String>::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
			stream << it->first << ": " << it->second << "\r\n";
	}
	stream << "\r\n";

	if (this->messageBody.length() != 0)
		stream << this->messageBody;

	String::getline(stream, str, '\0');
	return (str);
}

void	Response::setStatusCode(int statusCode) {
	this->statusCode = statusCode;

	switch (statusCode) {
		case 200:
			this->reasonPhrase = "OK";
			break ;
		case 304:
			this->reasonPhrase = "Not Modified";
			break ;
	}
}

void	Response::getFileContents(const String& file)
{
	std::ifstream	filestream(file.c_str());
	String			fileContents;
	String			str;

	while (String::getline(filestream, str))
	{
		fileContents += str;
		fileContents += "\n";
	}
	messageBody = fileContents;
}
