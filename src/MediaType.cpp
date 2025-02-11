/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaType.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:16:30 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 01:58:24 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>
#include "MediaType.hpp"

MediaType::MediaType(void) {}

MediaType::MediaType(const String &file) {
	static const String	whiteSpaces = "\t\n\v\f\r ";
	std::ifstream		ifs(file.c_str());
	String				str;
	String				mediaType;
	String				extension;

	if (ifs.is_open() == false)
		throw MIMETypesFileNotFound();

	String::getline(ifs, str);
	while (String::getline(ifs, str, ';')) {
		mediaType = str.trim(whiteSpaces);
		if (mediaType.length() == 0)
			break ;

		std::vector<String>	elements = mediaType.split(whiteSpaces);

		for (std::vector<String>::const_iterator it = elements.begin() + 1; it != elements.end(); it++)
			this->map.insert(std::make_pair(elements[0], *it));
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

MediaType::MIMETypesFileNotFound::MIMETypesFileNotFound(void) {}

MediaType::MIMETypesFileNotFound::~MIMETypesFileNotFound(void) throw() {}

const char	*MediaType::MIMETypesFileNotFound::what(void) const throw() {
	return ("Unable to locate mime.types file");
}
