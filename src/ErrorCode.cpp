/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/19 16:18:42 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Time.hpp"
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
	this->insert("Date", Time::printHTTPDate());
	this->insert("Content-Type", "text/html");
	this->processStage = Response::DONE;
}

ErrorCode::ErrorCode(
	float httpVersion,
	int statusCode,
	String reasonPhrase,
	const char *title) :
	Response()
{
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;
	this->insert("Content-Type", "application/problem+json");
	this->insert("Content-Language", "en");
	this->messageBody = "{\n\t\"title\": \"" + String(title) + "\"\n}";
	this->processStage = Response::DONE;
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
{
	this->insert("Connection", "close");	// Close connection for all bad requests for now
}

BadRequest400::BadRequest400(const char *title) :
	ErrorCode(1.1, 400, "Bad Request", title)
{
	this->insert("Connection", "close");	// Close connection for all bad requests for now
}

//	403 Forbidden
Forbidden403::Forbidden403(void) :
	ErrorCode(1.1, 403, "Forbidden")
{}

Forbidden403::Forbidden403(const char *title) :
	ErrorCode(1.1, 403, "Forbidden", title)
{}

//	404 Not Found
NotFound404::NotFound404(void) :
	ErrorCode(1.1, 404, "Not Found")
{}

NotFound404::NotFound404(const char *title) :
	ErrorCode(1.1, 404, "Not Found", title)
{}

//	405 Method Not Allowed
MethodNotAllowed405::MethodNotAllowed405(void) :
	ErrorCode(1.1, 405, "Method Not Allowed")
{}

MethodNotAllowed405::MethodNotAllowed405(const char *title) :
	ErrorCode(1.1, 405, "Method Not Allowed", title)
{}

//	412 Precondition Failed
PreconditionFailed412::PreconditionFailed412(void) :
	ErrorCode(1.1, 412, "Precondition Failed")
{}

PreconditionFailed412::PreconditionFailed412(const char *title) :
	ErrorCode(1.1, 412, "Precondition Failed", title)
{}

//	413 Content Too Large
ContentTooLarge413::ContentTooLarge413(void) :
	ErrorCode(1.1, 413, "Content Too Large")
{}

ContentTooLarge413::ContentTooLarge413(const char *title) :
	ErrorCode(1.1, 413, "Content Too Large", title)
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
