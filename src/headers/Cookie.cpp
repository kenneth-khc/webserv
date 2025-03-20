/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:41:12 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:06:25 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <vector>
#include "terminalValues.hpp"
#include "Response.hpp"
#include "Cookie.hpp"

Cookie::Cookie(void) :
	secure(false),
	httpOnly(false)
{}

Cookie::Cookie(const String &name, const String &value) :
	name(name),
	value(value),
	secure(false),
	httpOnly(false)
{}

Cookie::~Cookie(void) {}

Cookie::Cookie(const Cookie &obj) :
	name(obj.name),
	value(obj.value),
	expires(obj.expires),
	maxAge(obj.maxAge),
	domain(obj.domain),
	path(obj.path),
	secure(obj.secure),
	httpOnly(obj.httpOnly)
{}

Cookie	&Cookie::operator=(const Cookie &obj) {
	if (this == &obj)
		return (*this);
	this->name = obj.name;
	this->value = obj.value;
	this->expires = obj.expires;
	this->maxAge = obj.maxAge;
	this->domain = obj.domain;
	this->path = obj.path;
	this->secure = obj.secure;
	this->httpOnly = obj.httpOnly;
	return (*this);
}

void	Cookie::operator=(const String &value) {
	this->value = value;
}

bool	Cookie::operator==(const Cookie &obj) const {
	if (this->name == obj.name)
		return (true);
	else
		return (false);
}

String	Cookie::constructSetCookieHeader(void) const {
	String	setCookieHeader;

	setCookieHeader = this->name + "=" + this->value;
	if (this->expires.exists == true)
		setCookieHeader += "; Expires=" + this->expires.value;
	if (this->maxAge.exists == true) {
		std::stringstream	stream;

		stream << this->maxAge.value;
		setCookieHeader += "; Max-Age=" + stream.str();
	}
	if (this->domain.exists == true)
		setCookieHeader += "; Domain=" + this->domain.value;
	if (this->path.exists == true)
		setCookieHeader += "; Path=" + this->path.value;
	if (this->secure == true)
		setCookieHeader += "; Secure";
	if (this->httpOnly == true)
		setCookieHeader += "; HttpOnly";
	return (setCookieHeader);
}

bool	isCookieString(std::map<String, Cookie> &cookies, const String &line) {
	std::vector<String>			values = line.split("; ");
	std::map<String, Cookie>	validCookies;

	if (values.size() == 0)
		return (false);

	for (std::vector<String>::const_iterator it = values.begin(); it != values.end(); it++) {
		std::vector<String>	cookiePair = it->split("=");

		if (isCookiePair(*it) == true)
			validCookies.insert(std::make_pair(cookiePair[0], Cookie(cookiePair[0], cookiePair[1])));
	}
	if (validCookies.size() == 0)
		return (false);
	cookies = validCookies;
	return (true);
}

bool	isCookiePair(const String &line) {
	std::vector<String>	cookiePair = line.split("=");

	if (isToken(cookiePair[0]) == false)
		return (false);
	if (isCookieValue(cookiePair[1]) == false)
		return (false);
	return (true);
}

bool	isCookieValue(const String &line) {
	for (String::size_type i = 0; i < line.length(); i++) {
		if (isCookieOctet(line[i]) == false)
			return (false);
	}
	return (true);
}

bool	isCookieOctet(const unsigned &character) {
	const String	values = "\",;\\";

	if (std::isgraph(character) == 0)
		return (false);
	if (values.find(character).exists == true)
		return (false);
	return (true);
}
