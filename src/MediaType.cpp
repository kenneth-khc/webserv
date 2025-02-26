/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaType.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:16:30 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/26 22:31:54 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include "terminalValues.hpp"
#include "contentType.hpp"
#include "MediaType.hpp"

MediaType::MediaType(void) {}

MediaType::MediaType(const String &file) {
	if (file.length() == 0)
		throw MIMETypesFileError("Unable to locate MIME types file");

	const String		whiteSpaces = "\t\n\v\f\r ";
	std::ifstream		ifs(file.c_str());
	String				mediaType;
	String				extension;
	std::stringstream	stream;
	std::size_t			i = 0;

	if (ifs.is_open() == false)
		throw MIMETypesFileError("Unable to read MIME types file");

	while (String::getline(ifs, mediaType, ';')) {
		i++;
		stream.str("");
		stream << i;

		std::vector<String>	elements = mediaType.split(whiteSpaces);
		if (elements.size() < 2 || isMediaType(elements[0]) == false)
			throw MIMETypesFileError("MIME types file error at line " + stream.str());

		for (std::vector<String>::const_iterator it = elements.begin() + 1; it != elements.end(); it++) {
			if (isToken(*it) == false)
				throw MIMETypesFileError("MIME types file error at line " + stream.str());

			this->map.insert(std::make_pair(*it, elements[0]));
		}

		String::getline(ifs, mediaType);
		if (mediaType.length() != 0)
			throw MIMETypesFileError("MIME types file error at line " + stream.str());
	};
}

MediaType::~MediaType(void) {}

MediaType::MediaType(const MediaType &obj) :
	map(obj.map)
{}

MediaType	&MediaType::operator=(const MediaType &obj) {
	if (this == &obj)
		return (*this);
	this->map = obj.map;
	return (*this);
}

Optional<String>	MediaType::operator[](const String &key) {
	try {
		return (this->map.at(key));
	}
	catch (const std::out_of_range &e) {
		return (Optional<String>());
	}
}

const Optional<String>	MediaType::operator[](const String &key) const {
	try {
		return (this->map.at(key));
	}
	catch (const std::out_of_range &e) {
		return (Optional<String>());
	}
}

MediaType::MIMETypesFileError::MIMETypesFileError(const String &message) :
	message(message)
{}

MediaType::MIMETypesFileError::~MIMETypesFileError(void) throw() {}

const char	*MediaType::MIMETypesFileError::what(void) const throw() {
	return (message.c_str());
}
