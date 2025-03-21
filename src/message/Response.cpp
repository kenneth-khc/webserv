/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:20:18 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/05 17:23:24 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <sstream>
#include "ErrorCode.hpp"
#include "Response.hpp"

Response::Response(void) :
	Message(),
	flags(0)
{
	this->httpVersion = 1.1;
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

Response&	Response::operator=(const ErrorCode& obj)
{
	if (this == &obj)
		return (*this);

	std::multimap<String, String>::const_iterator	it = this->headers.find("Server");
	String											serverName = it != this->headers.end() ? it->second : "server";

	Message::operator=(obj);
	this->statusCode = obj.statusCode;
	this->reasonPhrase = obj.reasonPhrase;
	this->flags = obj.flags;

	std::stringstream	stream;
	stream << "<html>\n"
		   << 	"<head>"
		   << 		"<title>\n"
		   << 			this->statusCode << " " + this->reasonPhrase + "\n"
		   << 		"</title>\n"
		   << 	"</head>\n"
		   << 	"<body style=\"background-color:#f4dde7;font-family:'Comic Sans MS';\">\n"
		   << 		"<center><h1>\n"
		   << 			this->statusCode << " " + this->reasonPhrase + "\n"
		   << 		"</h1></center>\n"
		   << 		"<hr><center>\n"
		   << 			serverName + "\n"
		   << 		"</center>\n"
		   << 	"</body>\n"
		   << "</html>";
	this->messageBody = stream.str();
	this->insert("Content-Length", stream.str().length());
	return *this;
}


//	Turns the information stored in the Response instance into a complete
//	HTTP response message
const String	Response::toString(void) const {
	std::stringstream	stream;
	String				str;
	String				temp;

	stream << "HTTP/" << this->httpVersion << ' ';
	stream << this->statusCode << ' ';
	stream << this->reasonPhrase << "\r\n";

	if (this->headers.size() != 0) {
		for (std::multimap<String, String>::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
			stream << it->first << ": " << it->second << "\r\n";
	}
	stream << "\r\n";

	if (this->messageBody.length() != 0)
		stream << this->messageBody;

	while (String::getline(stream, temp)) {
		str += temp;
		str += "\n";
	};
	return (str);
}

void	Response::setStatusCode(int statusCode) {
	switch (statusCode) {
		case 200:
			this->reasonPhrase = "OK";
			break ;
		case 301:
			this->reasonPhrase = "Moved Permanently";
			break ;
		case 303:
			this->reasonPhrase = "See Other";
			break ;
		case 304:
			this->reasonPhrase = "Not Modified";
			break ;
		case 400:
			throw BadRequest400();
		case 404:
			throw NotFound404();
		case 412:
			throw PreconditionFailed412();
		case 415:
			throw UnsupportedMediaType415();
		case 500:
			throw InternalServerError500();
		case 501:
			throw NotImplemented501();
		case 505:
			throw VersionNotSupported505();
	}
	this->statusCode = statusCode;
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
