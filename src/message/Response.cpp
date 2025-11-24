/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:20:18 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/24 16:42:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ios>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include "contentType.hpp"
#include "ErrorCode.hpp"
#include "Request.hpp"
#include "Response.hpp"

Response::Response(void) :
	Message(),
	closeConnection(false)
{
	this->httpVersion = "1.1";
}

Response::~Response(void) {}

Response::Response(const Response &obj) :
	Message(obj),
	statusCode(obj.statusCode),
	reasonPhrase(obj.reasonPhrase),
	closeConnection(obj.closeConnection)
{}

Response&	Response::operator=(const Response& other)
{
	if (this == &other)
		return (*this);
	Message::operator=(other);
	this->statusCode = other.statusCode;
	this->reasonPhrase = other.reasonPhrase;
	this->closeConnection = other.closeConnection;
	return *this;
}

Response&	Response::operator=(const ErrorCode& obj)
{
	if (this == &obj)
		return (*this);

	std::multimap<String, String>::const_iterator	it = this->headers.find("Server");
	String											serverName = it != this->headers.end() ? it->second : "42webserv";

	Message::operator=(obj);
	this->statusCode = obj.statusCode;
	this->reasonPhrase = obj.reasonPhrase;
	this->closeConnection = obj.closeConnection;
	this->insert("Server", serverName);
	return *this;
}

void	Response::generateErrorPage(const Request &request) {
	struct stat	status;

	if (request.location->errorPages.find(this->statusCode) != request.location->errorPages.end()) {
		const String filepath = request.location->errorPages[this->statusCode];

		if (stat(filepath.c_str(), &status) == 0 && access(filepath.c_str(), R_OK) == 0) {
			this->getFileContents(filepath);
			this->insert("Content-Length", status.st_size);
			constructContentTypeHeader(*this, filepath, request.location->MIMEMappings);
			return ;
		} else {
			this->statusCode = 500;
			this->reasonPhrase = "Internal Server Error";
		}
	}

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
		   << 			this->find< Optional<String> >("server").value + "\n"
		   << 		"</center>\n"
		   << 	"</body>\n"
		   << "</html>";
	this->messageBody = stream.str();
	this->insert("Content-Length", stream.str().length());
}

/*
	Turns the information stored in the Response instance into a complete
	HTTP response message.
*/
bool	Response::isReady(void) {
	if (this->messageBody.length() == 0 && !(this->processStage & Response::DONE))
		return (false);

	if ((*this)["Content-Length"].exists == false) {
		Optional<String>	transferEncoding = (*this)["Transfer-Encoding"];

		if (transferEncoding.exists == true) {
			if (transferEncoding.value.find("chunked").exists == false)
				this->headers.find("Transfer-Encoding")->second += ", chunked";
		}
		else
			this->insert("Transfer-Encoding", "chunked");
	}

	std::stringstream	stream;
	String				temp;

	stream << "HTTP/" << std::setprecision(2) << this->httpVersion << ' ';
	stream << this->statusCode << ' ';
	stream << this->reasonPhrase << "\r\n";

	if (this->headers.size() != 0) {
		for (std::multimap<String, String>::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
			stream << it->first << ": " << it->second << "\r\n";
	}
	stream << "\r\n";

	this->formatted.append(stream.str().c_str(), stream.str().length());
	this->processStage |= Response::SEND_READY;
	return (true);
}

void	Response::appendMessageBody(void) {
	if (!(this->processStage & Response::DONE) && this->messageBody.length() == 0)
		return ;

	if ((*this)["Content-Length"].exists == true) {
		this->formatted.append(this->messageBody.c_str(), this->messageBody.length());
		this->messageBody.erase(0, this->messageBody.length());
		return ;
	}

	std::stringstream	chunkSize;

	chunkSize << std::hex << std::uppercase << this->messageBody.length();
	this->formatted.append(chunkSize.str().c_str(), chunkSize.str().length());
	this->formatted.append("\r\n", 2);
	this->formatted.append(this->messageBody.c_str(), this->messageBody.length());
	this->formatted.append("\r\n", 2);
	this->messageBody.erase(0, this->messageBody.length());

	if (this->processStage & Response::DONE && chunkSize.str() != "0" && this->messageBody.length() == 0)
		this->formatted.append("0\r\n\r\n", 5);
}

void	Response::setStatusCode(int statusCode) {
	switch (statusCode) {
		case 200:
			this->reasonPhrase = "OK";
			break ;
		case 204:
			this->reasonPhrase = "No Content";
			break ;
		case 301:
			this->reasonPhrase = "Moved Permanently";
			break ;
		case 302:
			this->reasonPhrase = "Found";
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
	char			buffer[1024];

	while (true)
	{
		std::streamsize	bytes = filestream.readsome(buffer, 1024);

		if (bytes == 0)
			return ;
		messageBody.append(buffer, bytes);
	}
}
