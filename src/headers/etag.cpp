/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etag.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:09:22 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/07 22:49:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ios>
#include <sstream>
#include <string>
#include "Response.hpp"
#include "Time.hpp"
#include "terminalValues.hpp"
#include "etag.hpp"

bool	isETagHeader(const std::string &line) {
	if (isEntityTag(line) == false)
		return (false);
	return (true);
}

bool	isEntityTag(const std::string &line) {
	std::string	opaqueTag = line;

	if (line.length() > 2 && line[0] == 'W' && line[1] == '/')
		opaqueTag = opaqueTag.substr(2);

	if (isOpaqueTag(opaqueTag) == false)
		return (false);
	return (true);
}

bool	isOpaqueTag(const std::string &line) {
	if (line.length() < 2 || line[0] != '"' || line[line.length() - 1] != '"')
		return (false);

	std::string	etagc = line.substr(1, line.length() - 2);

	if (isEntityTagCharacter(etagc) == false)
		return (false);
	return (true);
}

bool	isEntityTagCharacter(const std::string &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if ((line[i] != '"' && std::isgraph(line[i]) != 0) || isObsoleteText(line[i]))
			continue ;
		return (false);
	}
	return (true);
}

std::string	constructETagHeader(const struct timespec &lastModified, Optional<int> contentLength) {
	std::stringstream	stream;
	std::string			etag;

	stream << '"';
	stream << std::hex << Time::convertToGMT(lastModified.tv_sec);
	stream << '-';
	stream << std::hex << contentLength.value;
	stream << '"';
	std::getline(stream, etag, '\0');
	return (etag);
}

bool	compareETagStrong(const std::string &etagOne, const std::string &etagTwo) {
	if (etagOne == etagTwo)
		return (true);
	else
		return (false);
}

bool	compareETagWeak(std::string etagOne, std::string etagTwo) {
	if (etagOne.length() > 2 && etagOne[0] == 'W' && etagOne[1] == '/')
		etagOne = etagOne.substr(2);
	if (etagTwo.length() > 2 && etagTwo[0] == 'W' && etagTwo[1] == '/')
		etagTwo = etagTwo.substr(2);

	if (etagOne == etagTwo)
		return (true);
	else
		return (false);
}
