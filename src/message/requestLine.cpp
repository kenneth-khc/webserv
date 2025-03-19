/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:13:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/05 22:51:33 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "uri.hpp"
#include "path.hpp"
#include "ErrorCode.hpp"
#include "requestLine.hpp"

void	extractRequestLineComponents(const String &line, Request &request) {
	std::stringstream	stream(line);
	String				str;
	String				method;

	String::getline(stream, str, ' ');
	if (isToken(str) == false)
		throw BadRequest400();
	method = str;

	String::getline(stream, str, ' ');
	if (isRequestTarget(str) == false)
		throw BadRequest400();
	request.requestTarget = str;

	Optional<String::size_type>	queryPos = str.find("?");
	if (queryPos.exists == true) {
		request.path = str.substr(0, queryPos.value);
		request.query = Optional<String>(str.substr(queryPos.value + 1));

		const std::vector<String>	queries = request.query.value.split("&");
		for (std::vector<String>::const_iterator it = queries.begin(); it != queries.end(); it++) {
			std::vector<String>	split = it->split("=");

			request.queryPairs.insert(std::make_pair(split[0], split[1]));
		}
	}
	else
		request.path = str;

	String::getline(stream, str, '/');
	if (str != "HTTP")
		throw BadRequest400();
	String::getline(stream, str, '\0');
	if (str.length() != 3)
		throw BadRequest400();
	if (std::isdigit(str[0]) == 0 || str[1] != '.' || std::isdigit(str[2]) == 0)
		throw BadRequest400();
	std::stringstream(str) >> request.httpVersion;
	if (request.isValidMethod(method) == false)
		throw NotImplemented501();
	if (request.isSupportedVersion(request.httpVersion) == false)
		throw VersionNotSupported505();
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
