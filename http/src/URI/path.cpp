/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:05:10 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 11:54:14 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "path.hpp"

/********/
/* Path */
/********/
//	Serves to identify a resource within the scope of the URI's scheme and
//	authority. It generally corresponds to the acutal file path in the web
//	server's root directory.

//	
bool	isPathAbEmpty(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, '/');
	while (std::getline(stream, str, '/')) {
		if (isSegment(str) == false)
			return (false);	
	}
	return (true);
}

bool	isPathAbsolute(const std::string &line) {
	if (line[0] != '/')
		return (false);
	if (line.length() == 1)
		return (true);

	std::string	str = line.substr(1);
	if (isPathRootless(str) == false)
		return (false);
	return (true);
}

bool	isPathNoScheme(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, '/');
	if (isSegmentNZNC(str) == false)
		return (false);
	std::getline(stream, str);
	str = '/' + str;
	if (isPathAbEmpty(str) == false)
		return (false);
	return (true);
}

bool	isPathRootless(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;
	
	std::getline(stream, str, '/');
	if (isSegmentNZ(str) == false)
		return (false);
	std::getline(stream, str);
	str = '/' + str;
	if (isPathAbEmpty(str) == false)
		return (false);
	return (true);
}

bool	isPathEmpty(const std::string &line) {
	if (line.length() != 0)
		return (false);
	return (true);
}

bool	isAbsolutePath(const std::string &line) {
	if (line.length() == 0)
		return (false);

	if (isPathAbEmpty(line) == false)
		return (false);
	return (true);
}

bool	isSegment(const std::string &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if (isPrintableCharacter(line, i) == true)
			continue ;
		return (false);
	}
	return (true);
}

bool	isSegmentNZ(const std::string &line) {
	if (line.length() == 0)
		return (false);
	if (isSegment(line) == false)
		return (false);
	return (true);
}

bool	isSegmentNZNC(const std::string &line) {
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

bool	isPrintableCharacter(const std::string &line, std::size_t &index) {
	static const std::string	values = ":@";

	if (isUnreservedCharacter(line[index]) == true)
		return (true);
	if (isPercentEncoded(line, index) == true) {
		index += 2;
		return (true);
	}
	if (isSubDelimiter(line[index]) == true)
		return (true);
	if (values.find(line[index]) != std::string::npos)
		return (true);
	return (false);
}
