/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fields.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 06:32:37 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "ErrorCode.hpp"
#include "fields.hpp"

bool	isFieldVisibleCharacter(const unsigned char &character) {
	if (isVisibleCharacter(character) == true)
		return (true);
	if (isObsoleteText(character) == true)
		return (true);
	return (false);
}

bool	isFieldContent(const std::string &line) {
	static const std::string	values = " \t";
	std::size_t					length = line.length();

	if (length == 0 || values.find(line[length - 1]) != std::string::npos)
		return (false);

	for (std::size_t i = 0; i < line.length(); i++) {
		if (isFieldVisibleCharacter(line[i]) == true)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);
	}
	return (true);
}

bool	isFieldValue(const std::string &line) {
	if (line.length() == 0)
		return (true);
	if (isFieldContent(line) == false)
		return (false);
	return (true);
}

void	getFieldLine(const std::string &line, Request &request) {
	static const std::string	values = " \t";

	if (line.find(':') == std::string::npos)
		throw BadRequest400();

	std::stringstream	stream(line);
	std::string			str;
	std::string			fieldName;
	
	std::getline(stream, str, ':');
	if (isToken(str) == false)
		throw BadRequest400();
	fieldName = str;
	
	if (!std::getline(stream, str))
		return ;
	if (str.find("\r\n ") != std::string::npos || str.find("\r\n\t") != std::string::npos)
		throw BadRequest400("Invalid use of obsolete line folding in field value.");
		
	std::size_t	frontPos = str.find_first_not_of(values);
	std::size_t	backPos = str.find_last_not_of(values);
	str = str.substr(frontPos, backPos + 1);

	if (isFieldValue(str) == false)
		throw BadRequest400();
	request.headers.insert(std::pair<std::string, std::string>(fieldName, str));
}
