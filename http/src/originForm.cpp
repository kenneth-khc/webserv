/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   originForm.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:17:55 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:19:20 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "query.hpp"
#include "originForm.hpp"

bool	isOriginForm(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, '?');
	if (isAbsolutePath(str) == false)
		return (false);

	std::getline(stream, str);
	if (str.length() == 0)
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}