/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:02:09 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:48:37 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "path.hpp"
#include "uri.hpp"

bool	isHierPart(const std::string &line) {
	if (line.find("//") != 0) {
		if (isPathAbsolute(line) == true)
			return (true);
		if (isPathRootless(line) == true)
			return (true);
		if (isPathEmpty(line) == true)
			return (true);
	}
	else {
		std::stringstream	stream(line);
		std::string			str;

		std::getline(stream, str, '/');
		std::getline(stream, str, '/');
		std::getline(stream, str, '/');
		
	}
}

bool	isAbsolutePath(const std::string &line) {
	if (line.length() == 0)
		return (false);

	std::stringstream	stream(line);
	std::string			str;
	
	std::getline(stream, str, '/');
	while (std::getline(stream, str, '/')) {
		if (isSegment(str) == false)
			return (false);
	}
	return (true);
}
