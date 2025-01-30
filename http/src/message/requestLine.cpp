/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:13:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 22:33:41 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "uri.hpp"
#include "path.hpp"
#include "ErrorCode.hpp"
#include "requestLine.hpp"

void	extractRequestLineComponents(const std::string &line, Request &request) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, ' ');
	if (isToken(str) == false)
		throw BadRequest400();
	if (request.isValidMethod(str) == false)
		throw NotImplemented501();

	std::getline(stream, str, ' ');
	if (isRequestTarget(str) == false)
		throw BadRequest400();
	request.requestTarget = str;

	std::getline(stream, str, '/');
	if (str != "HTTP")
		throw BadRequest400();
	std::getline(stream, str, '\0');
	if (str.length() != 3)
		throw BadRequest400();
	if (std::isdigit(str[0]) == 0 || str[1] != '.' || std::isdigit(str[2]) == 0)
		throw BadRequest400();
	std::stringstream(str) >> request.httpVersion;
	if (request.isSupportedVersion(request.httpVersion) == false)
		throw VersionNotSupported505();
}

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const std::string &line) {
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

bool	isOriginForm(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, '?');
	if (isAbsolutePath(str) == false)
		return (false);

	if (!std::getline(stream, str))
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}

bool	isAbsoluteForm(const std::string &line) {
	if (isAbsoluteURI(line) == false)
		return (false);
	return (true);
}

bool	isAuthorityForm(const std::string &line) {
	if (isAuthority(line) == false)
		return (false);
	return (true);
}

bool	isAsteriskForm(const std::string &line) {
	if (line.length() == 1 && line[0] == '*')
		return (true);
	return (false);
}
