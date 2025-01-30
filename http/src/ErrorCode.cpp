/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 21:22:01 by cteoh            ###   ########.fr       */
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
const std::string &title)
{
	this->httpVersion = httpVersion;
	this->statusCode = statusCode;
	this->reasonPhrase = reasonPhrase;
	this->headers.insert(std::make_pair("Content-Type", "application/problem+json"));
	this->headers.insert(std::make_pair("Content-Language", "en"));
	this->messageBody = "{\n\t\"title\": \"" + title + "\"\n}";
}

ErrorCode::~ErrorCode(void) throw() {}

ErrorCode::ErrorCode(const ErrorCode &obj): Response(obj) {}

ErrorCode	&ErrorCode::operator=(const ErrorCode &obj) {
	if (this == &obj)
		return (*this);
	Response::operator=(obj);
}

/********************/
/* Client Error 4XX */
/********************/
//	400 Bad Request
BadRequest400::BadRequest400(void) : 
	ErrorCode(1.1, 400, "Bad Request")
{}

BadRequest400::BadRequest400(std::string title) : 
	ErrorCode(1.1, 400, "Bad Request", title)
{}

//	404 Not Found
NotFound404::NotFound404(void) :
	ErrorCode(1.1, 404, "Not Found")
{}

NotFound404::NotFound404(std::string title) :
	ErrorCode(1.1, 404, "Not Found", title)
{}

/********************/
/* Server Error 5XX */
/********************/
//	501 Not Implemented
NotImplemented501::NotImplemented501(void) : 
	ErrorCode(1.1, 501, "Not Implemented")
{}

NotImplemented501::NotImplemented501(std::string title) : 
	ErrorCode(1.1, 501, "Not Implemented", title)
{}

//	505 Version Not Supported
VersionNotSupported505::VersionNotSupported505(void) : 
	ErrorCode(1.1, 505, "Version Not Supported")
{}

VersionNotSupported505::VersionNotSupported505(std::string title) : 
	ErrorCode(1.1, 505, "Version Not Supported", title)
{}
