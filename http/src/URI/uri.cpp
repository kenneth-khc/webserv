/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:02:09 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 17:44:35 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "path.hpp"
#include "authority.hpp"
#include "uri.hpp"

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
		std::getline(stream, str);
		str = '/' + str;
		if (isPathAbEmpty(str) == false)
			return (false);
	}
	return (true);
}

bool	isAbsolutePath(const std::string &line) {
	if (line.length() == 0)
		return (false);

	if (isPathAbEmpty(line) == false)
		return (false);
	return (true);
}
