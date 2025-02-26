/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/27 00:31:28 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "ErrorCode.hpp"

ErrorCode::ErrorCode(void) : Response() {}

ErrorCode::ErrorCode(
	float httpVersion,
	int statusCode,
	String reasonPhrase) :
	Response()
{
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;

	this->insert("Content-Type", "text/html");
}

ErrorCode::ErrorCode(
	float httpVersion,
	int statusCode,
	String reasonPhrase,
	const char *title)
{
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;
	this->insert("Content-Type", "application/problem+json");
	this->insert("Content-Language", "en");
	this->messageBody = "{\n\t\"title\": \"" + String(title) + "\"\n}";
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

//	404 Not Found
NotFound404::NotFound404(void) :
	ErrorCode(1.1, 404, "Not Found")
{}

NotFound404::NotFound404(const char *title) :
	ErrorCode(1.1, 404, "Not Found", title)
{}

//	412 Precondition Failed
PreconditionFailed412::PreconditionFailed412(void) :
	ErrorCode(1.1, 412, "Precondition Failed")
{}

PreconditionFailed412::PreconditionFailed412(const char *title) :
	ErrorCode(1.1, 412, "Precondition Failed", title)
{}

//	415 Unsupported Media Type
UnsupportedMediaType415::UnsupportedMediaType415(void) :
	ErrorCode(1.1, 415, "Unsupported Media Type")
{}

UnsupportedMediaType415::UnsupportedMediaType415(const char *title) :
	ErrorCode(1.1, 412, "Unsupported Media Type", title)
{}

UnsupportedMediaType415::UnsupportedMediaType415(const char *header, const char *value) :
	ErrorCode(1.1, 415, "Unsupported Media Type")
{
	this->insert(header, value);
}

/********************/
/* Server Error 5XX */
/********************/
//	500 Internal Server Error
InternalServerError500::InternalServerError500(void) :
	ErrorCode(1.1, 500, "Internal Server Error")
{}

InternalServerError500::InternalServerError500(const char *title) :
	ErrorCode(1.1, 500, "Internal Server Error", title)
{}

//	501 Not Implemented
NotImplemented501::NotImplemented501(void) :
	ErrorCode(1.1, 501, "Not Implemented")
{}

NotImplemented501::NotImplemented501(const char *title) :
	ErrorCode(1.1, 501, "Not Implemented", title)
{}

//	505 Version Not Supported
VersionNotSupported505::VersionNotSupported505(void) :
	ErrorCode(1.1, 505, "Version Not Supported")
{}

VersionNotSupported505::VersionNotSupported505(const char *title) :
	ErrorCode(1.1, 505, "Version Not Supported", title)
{}
