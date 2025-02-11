/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:49:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/31 19:01:22 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <vector>
#include "terminalValues.hpp"
#include "host.hpp"

//	Subcomponent of the authority in a URI, identified by an IP address or a
//	registered name
bool	isHost(const std::string &line) {
	if (isIPLiteral(line) == true)
		return (true);
	if (isIPV4(line) == true)
		return (true);
	if (isRegName(line) == true)
		return (true);
	return (false);
}

bool	isIPLiteral(const std::string &line) {
	if (line[0] != '[' || line[line.length() - 1] != ']')
		return (false);

	std::string	str = line.substr(1, line.length() - 1);
	if (isIPV6(str) == true)
		return (true);
	if (isIPVFuture(str) == true)
		return (true);
	return (false);
}

bool	isIPV6(const std::string &line) {
	std::stringstream			stream(line);
	std::string					str;
	std::vector<std::string>	values;

	while (std::getline(stream, str, ':'))
		values.push_back(str);
	if (values.size() > 8)
		return (false);
	
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); it++) {
		if (it->length() == 0)
			continue ;
		if (it->find('.') != std::string::npos) {
			if (isIPV4(*it) == false)
				return (false);
		}
		else if (is16BitsHexa(*it) == false)
			return (false);
	}
	return (true);
}

bool	isIPVFuture(const std::string &line) {
	if (line.length() < 4)
		return (false);
	if (line[0] != 'v')
		return (false);

	std::size_t	dotPos = line.find('.');
	if (dotPos == std::string::npos || dotPos == 1)
		return (false);

	std::string	str = line.substr(1, dotPos);
	for (std::size_t i = 0; i < str.length(); i++) {
		if (std::isxdigit(str[i]) == 0)
			return (false);
	}

	str = line.substr(dotPos);
	if (str.length() == 0)
		return (false);
	for (std::size_t i = 0; i < str.length(); i++) {
		if (isUnreservedCharacter(str[i]) == true)
			continue ;
		if (isSubDelimiter(str[i]) == true)
			continue ;
		if (str[i] == ':')
			continue ;
		return (false);
	}
	return (true);
}

bool	isIPV4(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;
	int					counter = 0;

	while (std::getline(stream, str, '.')) {
		counter++;
		if (isDecOctet(str) == false)
			return (false);
	}
	if (counter != 4)
		return (false);
	return (true);
}

bool	is16BitsHexa(const std::string &line) {
	std::size_t	length = line.length();

	if (length == 0 || length > 4)
		return (false);

	for (std::size_t i = 0; i < length; i++) {
		if (std::isxdigit(line[i]) == 0)
			return (false);
	}
	return (true);
}

bool	isDecOctet(const std::string &line) {
	std::stringstream	stream(line);
	int					octetValue;

	stream >> octetValue;
	if (octetValue >= 0 && octetValue <= 255)
		return (true);
	return (false);
}

bool	isRegName(const std::string &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if (isUnreservedCharacter(line[i]) == true)
			continue ;
		if (isPercentEncoded(line, i) == true) {
			i += 2;
			continue ;
		}
		if (isSubDelimiter(line[i]) == true)
			continue ;
		return (false);
	}
	return (true);
}
