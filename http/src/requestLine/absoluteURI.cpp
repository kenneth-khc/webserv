/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absoluteURI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:22:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/26 02:45:35 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "scheme.hpp"
#include "query.hpp"
#include "uri.hpp"
#include "absoluteURI.hpp"

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

	if (!std::getline(stream, str))
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}
