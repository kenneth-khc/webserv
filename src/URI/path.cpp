/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:05:10 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 02:13:45 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "path.hpp"

/********/
/* Path */
/********/
//	Serves to identify a resource within the scope of the URI's scheme and
//	authority. It generally corresponds to the actual file path in the web
//	server's root directory.

//	Path that can be empty or begin with a '/'. This is checked when there is
//	an authority component.
//		http://example.com/hello/world
bool	isPathAbEmpty(const String &line) {
	std::stringstream	stream(line);
	String				str;

	String::getline(stream, str, '/');
	while (String::getline(stream, str, '/')) {
		if (isSegment(str) == false)
			return (false);
	}
	return (true);
}

//	Path that begins with a '/' but not '//'. This is checked when there is no
//	authority component.
//		/hello/world
bool	isPathAbsolute(const String &line) {
	if (line[0] != '/')
		return (false);
	if (line.length() == 1)
		return (true);
	if (line[1] == '/')
		return (false);

	String	str = line.substr(1);
	if (isPathRootless(str) == false)
		return (false);
	return (true);
}

//	Represents the path for URIs without scheme (starts without ':'). Can be
//	considered as the relative path.
//		hello/world
bool	isPathNoScheme(const String &line) {
	std::stringstream	stream(line);
	String			str;

	String::getline(stream, str, '/');
	if (isSegmentNZNC(str) == false)
		return (false);
	String::getline(stream, str, '\0');
	str = "/" + str;
	if (isPathAbEmpty(str) == false)
		return (false);
	return (true);
}

//	Paths that do not start with '/' and contains reserved characters like ':'
//	and '@'.
//		mailto:user@example.com
bool	isPathRootless(const String &line) {
	std::stringstream	stream(line);
	String				str;

	String::getline(stream, str, '/');
	if (isSegmentNZ(str) == false)
		return (false);
	String::getline(stream, str, '\0');
	str = "/" + str;
	if (isPathAbEmpty(str) == false)
		return (false);
	return (true);
}

bool	isPathEmpty(const String &line) {
	if (line.length() != 0)
		return (false);
	return (true);
}

//	Non-empty paths that must begin with a '/'. Used and checked within a
//	request target of a request message.
bool	isAbsolutePath(const String &line) {
	if (line.length() == 0)
		return (false);

	if (isPathAbEmpty(line) == false)
		return (false);
	return (true);
}

bool	isSegment(const String &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if (isPrintableCharacter(line, i) == true)
			continue ;
		return (false);
	}
	return (true);
}

bool	isSegmentNZ(const String &line) {
	if (line.length() == 0)
		return (false);
	if (isSegment(line) == false)
		return (false);
	return (true);
}

bool	isSegmentNZNC(const String &line) {
	if (line.length() == 0)
		return (false);

	for (std::size_t i = 0; i < line.length(); i++) {
		if (isUnreservedCharacter(line[i]) == true)
			continue ;
		if (isPercentEncoded(line, i) == true) {
			i += 2;
			continue ;
		}
		if (isSubDelimiter(line[i]) == true)
			continue ;
		if (line[i] == '@')
			continue ;
		return (false);
	}
	return (true);
}

bool	isPrintableCharacter(const String &line, std::size_t &index) {
	static const String	values = ":@";

	if (isUnreservedCharacter(line[index]) == true)
		return (true);
	if (isPercentEncoded(line, index) == true) {
		index += 2;
		return (true);
	}
	if (isSubDelimiter(line[index]) == true)
		return (true);
	if (values.find(line[index]) != String::npos)
		return (true);
	return (false);
}
