/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestFields.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 02:15:40 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "ErrorCode.hpp"
#include "headers.hpp"
#include "requestFields.hpp"

bool	isFieldVisibleCharacter(const unsigned char &character) {
	if (isVisibleCharacter(character) == true)
		return (true);
	if (isObsoleteText(character) == true)
		return (true);
	return (false);
}

bool	isFieldContent(const String &line) {
	static const String	values = " \t";
	std::size_t					length = line.length();

	if (length == 0 || values.find(line[length - 1]) != String::npos)
		return (false);

	for (std::size_t i = 0; i < line.length(); i++) {
		if (isFieldVisibleCharacter(line[i]) == true)
			continue ;
		if (values.find(line[i]) != String::npos)
			continue ;
		return (false);
	}
	return (true);
}

bool	isFieldValue(const String &line) {
	if (line.length() == 0)
		return (true);
	if (isFieldContent(line) == false)
		return (false);
	return (true);
}

void	extractFieldLineComponents(const String &line, Request &request) {
	static const String	optionalWhiteSpaces = " \t";

	if (line.find(':') == String::npos)
		throw BadRequest400();

	std::stringstream	stream(line);
	String			str;
	String			fieldName;

	String::getline(stream, str, ':');
	if (isToken(str) == false)
		throw BadRequest400();
	fieldName = str;

	if (!String::getline(stream, str, '\0'))
		return ;
	if (str.find("\r\n ") != String::npos || str.find("\r\n\t") != String::npos)
		throw BadRequest400("Invalid use of obsolete line folding in field value.");

	str = str.trim(optionalWhiteSpaces);

	if (isFieldValue(str) == false)
		throw BadRequest400();
	fieldName = Request::stringToLower(fieldName);
	if (checkMandatoryHeaders(fieldName, str) == false)
		throw BadRequest400();
	request.insert(fieldName, str);
}
