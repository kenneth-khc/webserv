/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestTarget.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:16:19 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 18:29:52 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "query.hpp"
#include "uri.hpp"
#include "absoluteURI.hpp"
#include "authority.hpp"
#include "requestTarget.hpp"

bool	isAsteriskForm(const std::string &line) {
	if (line.length() == 1 && line[0] == '*')
		return (true);
	return (false);
}

bool	isAuthorityForm(const std::string &line) {
	if (isAuthority(line) == false)
		return (false);
	return (true);
}

bool	isAbsoluteForm(const std::string &line) {
	if (isAbsoluteURI(line) == false)
		return (false);
	return (true);
}

bool	isOriginForm(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, '?');
	if (isAbsolutePath(str) == false)
		return (false);

	if (!std::getline(stream, str))
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}

bool	isRequestTarget(const std::string &line) {
	if (isOriginForm(line) == true)
		return (true);
	if (isAbsoluteForm(line) == true)
		return (true);
	if (isAuthorityForm(line) == true)
		return (true);
	if (isAsteriskForm(line) == true)
		return (true);
	return (false);
}
