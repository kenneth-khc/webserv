/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaType.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:16:30 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 05:41:33 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>
#include "MediaType.hpp"

MediaType::MediaType(void) {}

MediaType::MediaType(const std::string &file) {
	static const std::string	whiteSpaces = "\t\n\v\f\r ";
	std::ifstream				ifs(file.c_str());
	std::string					str;
	std::size_t					startPos;
	std::size_t					endPos;
	std::string					mediaType;
	std::string					extension;

	if (ifs.is_open() == false)
		throw MIMETypesNotFound();
	std::getline(ifs, str);
	while (std::getline(ifs, str, ';')) {
		startPos = str.find_first_not_of(whiteSpaces);
		endPos = str.find_first_of(whiteSpaces, startPos);
		if (endPos == std::string::npos)
			break ;
		mediaType = str.substr(startPos, endPos - startPos);

		while (true) {
			startPos = str.find_first_not_of(whiteSpaces, endPos);
			endPos = str.find_first_of(whiteSpaces, startPos);

			if (endPos != std::string::npos)
				extension = str.substr(startPos, endPos - startPos);
			else
				extension = str.substr(startPos);

			this->map.insert(std::make_pair(extension, mediaType));

			if (endPos == std::string::npos)
				break ;
		}
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

Optional<std::string>	MediaType::operator[](const std::string &key) {
	try {
		return (this->map.at(key));
	}
	catch (const std::out_of_range &e) {
		return (Optional<std::string>());
	}
}

const Optional<std::string>	MediaType::operator[](const std::string &key) const {
	try {
		return (this->map.at(key));
	}
	catch (const std::out_of_range &e) {
		return (Optional<std::string>());
	}
}

MediaType::MIMETypesNotFound::MIMETypesNotFound(void) {}

MediaType::MIMETypesNotFound::~MIMETypesNotFound(void) throw() {}

const char	*MediaType::MIMETypesNotFound::what(void) const throw() {
	return ("Unable to locate mime.types file");
}
