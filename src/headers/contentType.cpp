/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:09:40 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:05:45 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Optional.hpp"
#include "terminalValues.hpp"
#include "contentType.hpp"

bool	isContentTypeHeader(const String &line) {
	if (isMediaType(line) == false)
		return (false);
	return (true);
}

bool	isMediaType(const String &line) {
	Optional<String::size_type>	pos = line.find('/');

	if (pos.exists == false)
		return (false);

	std::stringstream	stream(line);
	String				str;

	String::getline(stream, str, '/');
	if (isToken(str) == false)
		return (false);
	pos = line.find(';', pos.value + 1);	// No support for media type parameters
	if (pos.exists == true)
		String::getline(stream, str, ';');
	else
		String::getline(stream, str);
	if (isToken(str) == false)
		return (false);
	return (true);
}

void	constructContentTypeHeader(
	Response &response,
	const String &file,
	const MediaType &MIMEMappings)
{
	Optional<String::size_type>	pos = file.find('.');
	String						extension = file.substr(pos.value + 1);
	Optional<String>			mediaType = MIMEMappings[extension];

	if (mediaType.exists == false)
		response.insert("Content-Type", "application/octet-stream");
	else
		response.insert("Content-Type", mediaType.value);
}
