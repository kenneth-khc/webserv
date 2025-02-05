/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:02:09 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 18:14:39 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "host.hpp"
#include "path.hpp"
#include "uri.hpp"

/***************/
/* General URI */
/***************/
//	A typical URL/URI consists of a hierarchical sequence of components,
//	including:
//		'scheme':'hier-part' with optional [ ?'query' ] and [ #'fragment' ]
//
//	The 'fragment' part is not sent as part of the request to the server, so a
//	server does not use the information to process the request and only the
//	client will make use of it. It is included here for the sake of
//	completeness of a URL/URI.
bool	isAbsoluteURI(const std::string &line) {
	if (line.find(':') == std::string::npos)
		return (false);

	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, ':');
	if (isScheme(str) == false)
		return (false);

	std::getline(stream, str, '?');
	if (isHierPart(str) == false)
		return (false);

	if (!std::getline(stream, str, '\0'))
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}

/**********/
/* Scheme */
/**********/
//	Refers to the specification/protocol to be used in processing the URI.
bool	isScheme(const std::string &line) {
	static const std::string	values = "+-.";

	if (std::isalpha(line[0]) == 0)
		return (false);

	for (std::size_t i = 1; i < line.length(); i++) {
		if (std::isalpha(line[i]) != 0)
			continue ;
		if (std::isdigit(line[i]) != 0)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);
	}
	return (true);
}

/*************/
/* Hier-Part */
/*************/
//	'hier-part' (hierarchical part) represents the authority (domain/IP and
//	port) and path of the URI.
bool	isHierPart(const std::string &line) {
	std::size_t	doubleSlashPos = line.find("//");

	if (doubleSlashPos == std::string::npos) {
		if (isPathAbsolute(line) == true)
			return (true);
		if (isPathRootless(line) == true)
			return (true);
		if (isPathEmpty(line) == true)
			return (true);
	}
	else {
		std::string	str = line.substr(doubleSlashPos + 2);
		std::stringstream	stream(str);

		std::getline(stream, str, '/');
		if (isAuthority(str) == false)
			return (false);
		std::getline(stream, str, '\0');
		str = '/' + str;
		if (isPathAbEmpty(str) == false)
			return (false);
	}
	return (true);
}

bool	isAuthority(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, ':');
	if (isHost(str) == false)
		return (false);
	if (!std::getline(stream, str, '\0'))
		return (true);
	if (isPort(str) == false)
		return (false);
	return (true);
}

bool	isPort(const std::string &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if (std::isdigit(line[i]) == 0)
			return (false);
	}
	return (true);
}

/*********/
/* Query */
/*********/
//	Used to help (along with the path) identify the specific resource being
//	requested, by acting as information to be used in processing.
bool	isQuery(const std::string &line) {
	static const std::string	values = "/?";

	for (std::size_t i = 0; i < line.length(); i++) {
		if (isPrintableCharacter(line, i) == true)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);
	}
	return (true);
}

/************/
/* Fragment */
/************/
//	Allows indirect indetification of a secondary resource by reference to a
//	primary resource. Example of secondary resource is a webpage anchor in an
//	HTML, which represents a specific view of the document.
bool	isFragment(const std::string &line) {
	static const std::string	values = "/?";

	for (std::size_t i = 0; i < line.length(); i++) {
		if (isPrintableCharacter(line, i) == true)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);
	}
	return (true);
}
