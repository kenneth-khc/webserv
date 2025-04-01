/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineState.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:09:36 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 16:52:20 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <ios>
#include <sstream>
#include "Optional.hpp"
#include "ErrorCode.hpp"
#include "terminalValues.hpp"
#include "uri.hpp"
#include "path.hpp"
#include "Client.hpp"
#include "HeadersState.hpp"
#include "RequestLineState.hpp"

String	RequestLineState::percentDecodeString(const String &line) const {
	String	decodedString;

	for (String::size_type i = 0; i < line.length(); i++) {
		if (line[i] == '%') {
			String::size_type	c;

			std::stringstream(line.substr(i + 1, 2)) >> std::hex >> c;
			decodedString.push_back(c);
			i += 2;
		}
		else if (line[i] == '+')
			decodedString.push_back(' ');
		else
			decodedString.push_back(line[i]);
	}

	return (decodedString);
}

RequestState	*RequestLineState::process(Request &request, Client &client) {
	String						&message = client.message;
	Optional<String::size_type>	terminatorPos = message.find("\r\n");

	if (terminatorPos.exists == false)
		return (this);

	std::stringstream			stream(message.substr(0, terminatorPos.value));
	String						str;
	String						method;

	String::getline(stream, str, ' ');
	if (isToken(str) == false)
		throw BadRequest400();
	request.method = str;

	String::getline(stream, str, ' ');
	if (isRequestTarget(str) == false)
		throw BadRequest400();
	request.requestTarget = str;

	Optional<String::size_type>	queryPos = str.find("?");
	if (queryPos.exists == true) {
		request.path = str.substr(0, queryPos.value);
		request.decodedPath = RequestLineState::percentDecodeString(request.path);

		request.query = Optional<String>(str.substr(queryPos.value + 1));
		request.decodedQuery.exists = true;
		request.decodedQuery.value = RequestLineState::percentDecodeString(request.query.value);

		const std::vector<String>	queries = request.decodedQuery.value.split("&");
		for (std::vector<String>::const_iterator it = queries.begin(); it != queries.end(); it++) {
			std::vector<String>	split = it->split("=");

			request.queryPairs.insert(std::make_pair(split[0], split[1]));
		}
	}
	else {
		request.path = str;
		request.decodedPath = RequestLineState::percentDecodeString(str);
	}

	String::getline(stream, str, '/');
	if (str != "HTTP")
		throw BadRequest400();
	String::getline(stream, str, '\0');
	if (str.length() != 3)
		throw BadRequest400();
	if (std::isdigit(str[0]) == 0 || str[1] != '.' || std::isdigit(str[2]) == 0)
		throw BadRequest400();
	std::stringstream(str) >> request.httpVersion;

	message.erase(0, terminatorPos.value);
	return (new HeadersState());
}

int	RequestLineState::getState(void) const {
	return (RequestState::REQUEST_LINE);
}

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const String &line) {
	if (isOriginForm(line) == true)
		return (true);
	if (isAbsoluteForm(line) == true)
		return (true);
	if (isAuthorityForm(line) == true)
		return (true);
	if (isAsteriskForm(line) == true)
		return (true);
	return (false);
}

bool	isOriginForm(const String &line) {
	std::stringstream	stream(line);
	String				str;

	String::getline(stream, str, '?');
	if (isAbsolutePath(str) == false)
		return (false);

	if (!String::getline(stream, str))
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}

bool	isAbsoluteForm(const String &line) {
	if (isAbsoluteURI(line) == false)
		return (false);
	return (true);
}

bool	isAuthorityForm(const String &line) {
	if (isAuthority(line) == false)
		return (false);
	return (true);
}

bool	isAsteriskForm(const String &line) {
	if (line.length() == 1 && line[0] == '*')
		return (true);
	return (false);
}
