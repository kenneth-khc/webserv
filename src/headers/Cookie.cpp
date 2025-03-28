/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:41:12 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/06 00:21:09 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "terminalValues.hpp"
#include "Response.hpp"
#include "Cookie.hpp"

Cookie::Cookie(void) :
	maxAge(0),
	secure(false),
	httpOnly(false)
{}

Cookie::Cookie(const String &name, const String &value) :
	name(name),
	value(value),
	maxAge(0),
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
	if (this->name != obj.name)
		return (*this);
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

bool	Cookie::operator==(const Cookie &obj) {
	if (this->name == obj.name)
		return (true);
	else
		return (false);
}

bool	isCookieString(const String &line, std::map<String, Cookie> &cookies) {
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

void	constructSetCookieHeader(Response &response, const std::vector<Cookie> &cookies) {
	String	setCookieHeader;

	for (std::vector<Cookie>::const_iterator it = cookies.begin(); it != cookies.end(); it++) {
		setCookieHeader = it->name + "=" + it->value;
		//	Append cookie attributes here
		response.insert("Set-Cookie", setCookieHeader);
	}
}
