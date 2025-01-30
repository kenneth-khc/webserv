/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 11:19:35 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Request.hpp"

const std::string	Request::methods[NUM_OF_METHODS] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"OPTIONS",
	"DELETE"
};

const float	Request::supportedVersions[NUM_OF_VERSIONS] = { 1.1 };

Request::Request(void) {}

Request::~Request(void) {}

Request::Request(const Request &obj) :
	method(obj.method),
	requestTarget(obj.requestTarget),
	httpVersion(obj.httpVersion),
	headers(obj.headers),
	messageBody(obj.messageBody)
{}

Request	&Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	this->method = obj.method;
	this->requestTarget = obj.requestTarget;
	this->httpVersion = obj.httpVersion;
	this->headers = obj.headers;
	this->messageBody = obj.messageBody;
}

bool	Request::isValidMethod(const std::string &method) {
	for (int i = 0; i < NUM_OF_METHODS; i++) {
		if (method == methods[i]) {
			this->method = i;
			return (true);
		}
	}
	return (false);
}

bool	Request::isSupportedVersion(const float &version) {
	for (int i = 0; i < NUM_OF_VERSIONS; i++) {
		if (version > supportedVersions[i])
			return (false);
	}
	return (true);
}
