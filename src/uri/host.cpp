/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:49:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/19 23:58:02 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <vector>
#include "Optional.hpp"
#include "terminalValues.hpp"
#include "host.hpp"

//	Subcomponent of the authority in a URI, identified by an IP address or a
//	registered name
bool	isHost(const String &line) {
	if (isIPLiteral(line) == true)
		return (true);
	if (isIPV4(line) == true)
		return (true);
	if (isRegName(line) == true)
		return (true);
	return (false);
}

bool	isIPLiteral(const String &line) {
	if (line[0] != '[' || line[line.length() - 1] != ']')
		return (false);

	String	str = line.substr(1, line.length() - 1);
	if (isIPV6(str) == true)
		return (true);
	if (isIPVFuture(str) == true)
		return (true);
	return (false);
}

bool	isIPV6(const String &line) {
	std::stringstream		stream(line);
	String					str;
	std::vector<String>		values;

	while (String::getline(stream, str, ':'))
		values.push_back(str);
	if (values.size() > 8)
		return (false);

	for (std::vector<String>::const_iterator it = values.begin(); it != values.end(); it++) {
		if (it->length() == 0)
			continue ;
		if (it->find('.').exists == true) {
			if (isIPV4(*it) == false)
				return (false);
		}
		else if (is16BitsHexa(*it) == false)
			return (false);
	}
	return (true);
}

bool	isIPVFuture(const String &line) {
	if (line.length() < 4)
		return (false);
	if (line[0] != 'v')
		return (false);

	Optional<String::size_type>	dotPos = line.find('.');
	if (dotPos.exists == false || dotPos.value == 1)
		return (false);

	String	str = line.substr(1, dotPos.value);
	for (String::size_type i = 0; i < str.length(); i++) {
		if (std::isxdigit(str[i]) == 0)
			return (false);
	}

	str = line.substr(dotPos.value);
	if (str.length() == 0)
		return (false);
	for (String::size_type i = 0; i < str.length(); i++) {
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

bool	isIPV4(const String &line) {
	std::stringstream	stream(line);
	String				str;
	int					counter = 0;

	while (String::getline(stream, str, '.')) {
		counter++;
		if (isDecOctet(str) == false)
			return (false);
	}
	if (counter != 4)
		return (false);
	return (true);
}

bool	is16BitsHexa(const String &line) {
	String::size_type	length = line.length();

	if (length == 0 || length > 4)
		return (false);

	for (String::size_type i = 0; i < length; i++) {
		if (std::isxdigit(line[i]) == 0)
			return (false);
	}
	return (true);
}

bool	isDecOctet(const String &line) {
	int	octetValue;

	std::stringstream(line) >> octetValue;
	if (octetValue >= 0 && octetValue <= 255)
		return (true);
	return (false);
}

bool	isRegName(const String &line) {
	for (String::size_type i = 0; i < line.length(); i++) {
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
