/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/08 16:07:05 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ErrorCode.hpp"

ErrorCode::ErrorCode(void) {}

ErrorCode::ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase) {
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;
}

ErrorCode::ErrorCode(
float httpVersion,
int statusCode,
std::string reasonPhrase,
const char *title)
{
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;
	this->headers.insert(std::make_pair("Content-Type", "application/problem+json"));
	this->headers.insert(std::make_pair("Content-Language", "en"));
	this->messageBody = "{\n\t\"title\": \"" + std::string(title) + "\"\n}";
}

ErrorCode::ErrorCode(
float httpVersion,
int statusCode,
std::string reasonPhrase,
const std::string &messageBody)
{
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;
	this->messageBody = messageBody;
}

ErrorCode::~ErrorCode(void) throw() {}

ErrorCode::ErrorCode(const ErrorCode &obj): Response(obj) {}

ErrorCode	&ErrorCode::operator=(const ErrorCode &obj) {
	if (this == &obj)
		return (*this);
	Response::operator=(obj);
	return *this;
}

/********************/
/* Client Error 4XX */
/********************/
//	400 Bad Request
BadRequest400::BadRequest400(void) :
	ErrorCode(1.1, 400, "Bad Request")
{}

BadRequest400::BadRequest400(const char *title) :
	ErrorCode(1.1, 400, "Bad Request", title)
{}

BadRequest400::BadRequest400(const std::string &messageBody) :
	ErrorCode(1.1, 400, "Bad Request", messageBody)
{}

//	404 Not Found
NotFound404::NotFound404(void) :
	ErrorCode(1.1, 404, "Not Found")
{}

NotFound404::NotFound404(const char *title) :
	ErrorCode(1.1, 404, "Not Found", title)
{}

NotFound404::NotFound404(const std::string &messageBody) :
	ErrorCode(1.1, 404, "Not Found", messageBody)
{}

//	412 Precondition Failed
PreconditionFailed412::PreconditionFailed412(void) :
	ErrorCode(1.1, 412, "Precondition Failed")
{}

PreconditionFailed412::PreconditionFailed412(const char *title) :
	ErrorCode(1.1, 412, "Precondition Failed", title)
{}

PreconditionFailed412::PreconditionFailed412(const std::string &messageBody) :
	ErrorCode(1.1, 412, messageBody)
{}

/********************/
/* Server Error 5XX */
/********************/
//	501 Not Implemented
NotImplemented501::NotImplemented501(void) :
	ErrorCode(1.1, 501, "Not Implemented")
{}

NotImplemented501::NotImplemented501(const char *title) :
	ErrorCode(1.1, 501, "Not Implemented", title)
{}

NotImplemented501::NotImplemented501(const std::string &messageBody) :
	ErrorCode(1.1, 501, "Not Implemented", messageBody)
{}

//	505 Version Not Supported
VersionNotSupported505::VersionNotSupported505(void) :
	ErrorCode(1.1, 505, "Version Not Supported")
{}

VersionNotSupported505::VersionNotSupported505(const char *title) :
	ErrorCode(1.1, 505, "Version Not Supported", title)
{}

VersionNotSupported505::VersionNotSupported505(const std::string &messageBody) :
	ErrorCode(1.1, 505, "Version Not Supported", messageBody)
{}
