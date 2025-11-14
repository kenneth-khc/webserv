/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etag.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:09:22 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/19 23:54:02 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Time.hpp"
#include "terminalValues.hpp"
#include "etag.hpp"

bool	isETagHeader(const String &line) {
	if (isEntityTag(line) == false)
		return (false);
	return (true);
}

bool	isEntityTag(const String &line) {
	String	opaqueTag = line;

	if (line.length() > 2 && line[0] == 'W' && line[1] == '/')
		opaqueTag = opaqueTag.substr(2);

	if (isOpaqueTag(opaqueTag) == false)
		return (false);
	return (true);
}

bool	isOpaqueTag(const String &line) {
	if (line.length() < 2 || line[0] != '"' || line[line.length() - 1] != '"')
		return (false);

	String	etagc = line.substr(1, line.length() - 2);

	if (isEntityTagCharacter(etagc) == false)
		return (false);
	return (true);
}

bool	isEntityTagCharacter(const String &line) {
	for (String::size_type i = 0; i < line.length(); i++) {
		if ((line[i] != '"' && std::isgraph(line[i]) != 0) || isObsoleteText(line[i]))
			continue ;
		return (false);
	}
	return (true);
}

String	constructETagHeader(const struct timespec &lastModified, Optional<int> contentLength) {
	std::stringstream	stream;

	stream << '"';
	stream << std::hex << Time::convertToGMT(lastModified.tv_sec);
	stream << '-';
	stream << std::hex << contentLength.value;
	stream << '"';
	return (stream.str());
}

bool	compareETagStrong(const String &etagOne, const String &etagTwo) {
	if (etagOne == etagTwo)
		return (true);
	else
		return (false);
}

bool	compareETagWeak(String etagOne, String etagTwo) {
	if (etagOne.length() > 2 && etagOne[0] == 'W' && etagOne[1] == '/')
		etagOne = etagOne.substr(2);
	if (etagTwo.length() > 2 && etagTwo[0] == 'W' && etagTwo[1] == '/')
		etagTwo = etagTwo.substr(2);

	if (etagOne == etagTwo)
		return (true);
	else
		return (false);
}
