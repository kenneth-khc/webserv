/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminalValues.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:04:21 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 11:50:15 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminalValues.hpp"

//	A string containing one or more symbols listed below, digits, and/or
//	alphabet characters.
bool	isToken(const std::string &line) {
	static const std::string	values = "!#$%&'*+-.^_`|~";

	if (line.length() == 0)
		return (false);
	for (std::size_t i = 0; i < line.length(); i++) {
		if (std::isalnum(line[i]) != 0)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);
	}
	return (true);
}

//	A character that is alphanumeric or one of the symbols listed below.
bool	isUnreservedCharacter(const unsigned char &character) {
	static const std::string	values = "-._~";

	if (std::isalnum(character) != 0)
		return (true);
	if (values.find(character) != std::string::npos)
		return (true);
	return (false);
}

//	A string that is made up of '%' and two hexadecimal characters, which
//	represents a symbol.
bool	isPercentEncoded(const std::string &line, int index) {
	if (std::string(&line[index]).length() < 3)
		return (false);
	if (line[index] != '%')
		return (false);
	if (std::isxdigit(line[index + 1]) == 0)
		return (false);
	if (std::isxdigit(line[index + 2]) == 0)
		return (false);
	return (true);
}

//	A character that is one of the symbols listed below.
bool	isSubDelimiter(const unsigned char &character) {
	static const std::string	values = "!$&'()*+,;=";

	if (values.find(character) != std::string::npos)
		return (true);
	return (false);
}

//	Any visible characters except space.
bool	isVisibleCharacter(const unsigned char &character) {
	if (std::isgraph(character) == 0)
		return (false);
	return (true);
}

//	Single-byte characters that are out of the ASCII range.
bool	isObsoleteText(const unsigned char &character) {
	if (character < 128)
		return (false);
	return (true);
}
