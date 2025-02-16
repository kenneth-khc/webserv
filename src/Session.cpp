/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 22:47:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/16 16:07:44 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base64.hpp"
#include "Time.hpp"
#include "Session.hpp"

const String Session::allowedCopyableCookies[NUM_OF_COOKIES] = {
	"lang"
};

Session::Session(void) {
	cookies.push_back(Cookie("sid", Base64::encode(Time::printHTTPDate())));
	cookies.push_back(Cookie("lang", "en"));
}

Session::~Session(void) {}

Session::Session(const Session &obj) :
	cookies(obj.cookies)
{}

Session	&Session::operator=(const Session &obj) {
	if (this == &obj)
		return (*this);
	this->cookies = obj.cookies;
	return (*this);
}

Optional<String>	Session::operator[](const String &cookieName) {
	for (std::vector<Cookie>::const_iterator it = this->cookies.begin(); it != this->cookies.end(); it++) {
		if (it->name == cookieName)
			return (Optional<String>(it->value));
	}
	return (Optional<String>());
}

const Optional<String>	Session::operator[](const String &cookieName) const {
	for (std::vector<Cookie>::const_iterator it = this->cookies.begin(); it != this->cookies.end(); it++) {
		if (it->name == cookieName)
			return (Optional<String>(it->value));
	}
	return (Optional<String>());
}

std::vector<Cookie>::iterator	Session::find(const String &key) {
	std::vector<Cookie>::iterator	it = this->cookies.begin();

	while (it != this->cookies.end()) {
		if (it->name == key)
			break ;
		it++;
	}
	return (it);
}

std::vector<Cookie>::const_iterator	Session::find(const String &key) const {
	std::vector<Cookie>::const_iterator	it = this->cookies.begin();

	while (it != this->cookies.end()) {
		if (it->name == key)
			break ;
		it++;
	}
	return (it);
}

bool	Session::operator==(const Session &obj) const {
	std::vector<Cookie>::const_iterator	cookiesOne = this->find("sid");
	std::vector<Cookie>::const_iterator	cookiesTwo = obj.find("sid");

	if (cookiesOne == this->cookies.end() || cookiesTwo == obj.cookies.end())
		return (false);

	//	Assume all other cookies in the two sessions are the same
	return (cookiesOne->value == cookiesTwo->value);
}

bool	Session::operator!=(const Session &obj) const {
	std::vector<Cookie>::const_iterator	cookiesOne = this->find("sid");
	std::vector<Cookie>::const_iterator	cookiesTwo = obj.find("sid");

	if (cookiesOne == this->cookies.end() || cookiesTwo == obj.cookies.end())
		return (true);

	//	Assume all other cookies in the two sessions are the same
	return (cookiesOne->value != cookiesTwo->value);
}

std::vector<Cookie>	Session::updateSession(const Session &obj) {
	std::vector<Cookie>	updatedCookies;

	if (*this != obj)
		return (updatedCookies);

	std::vector<Cookie>::iterator 		cookiesOne;
	std::vector<Cookie>::const_iterator cookiesTwo;
	for (int i = 0; i < NUM_OF_NAMES; i++) {
		cookiesOne = this->find(Session::allowedCopyableCookies[i]);
		cookiesTwo = obj.find(Session::allowedCopyableCookies[i]);

		if (cookiesOne != this->cookies.end()
			&& cookiesTwo != obj.cookies.end()
			&& cookiesOne->value != cookiesTwo->value) {
			cookiesOne->value = cookiesTwo->value;
			updatedCookies.push_back(*cookiesTwo);
		}
	}
	return (updatedCookies);
}
