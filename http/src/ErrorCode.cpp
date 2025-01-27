/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 05:18:00 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ErrorCode.hpp"

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
	this->headers.insert(std::pair<std::string, std::string>("Content-Type", "application/problem+json"));
	this->headers.insert(std::pair<std::string, std::string>("Content-Language", "en"));
	this->messageBody = "{\n\t\"title\": \"" + title + "\"\n}";
}

ErrorCode::ErrorCode(const ErrorCode &obj) {
	this->httpVersion = obj.httpVersion;
	this->statusCode = obj.statusCode;
	this->reasonPhrase = obj.reasonPhrase;
	this->headers = obj.headers;
	this->messageBody = obj.messageBody;
}

ErrorCode::~ErrorCode(void) throw() {}

/*******************/
/* 400 Bad Request */
/*******************/
BadRequest400::BadRequest400(void) : 
	ErrorCode(1.1, 400, "Bad Request")
{}

BadRequest400::BadRequest400(std::string title) : 
	ErrorCode(1.1, 400, "Bad Request", title)
{}

/***********************/
/* 501 Not Implemented */
/***********************/
NotImplemented501::NotImplemented501(void) : 
	ErrorCode(1.1, 501, "Not Implemented")
{}

NotImplemented501::NotImplemented501(std::string title) : 
	ErrorCode(1.1, 501, "Not Implemented", title)
{}

/*****************/
/* 404 Not Found */
/*****************/
NotFound404::NotFound404(void) :
	ErrorCode(1.1, 404, "Not Found")
{}

/*****************************/
/* 505 Version Not Supported */
/*****************************/
VersionNotSupported505::VersionNotSupported505(void) : 
	ErrorCode(1.1, 505, "Version Not Supported")
{}

VersionNotSupported505::VersionNotSupported505(std::string title) : 
	ErrorCode(1.1, 505, "Version Not Supported", title)
{}
