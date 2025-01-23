/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absoluteURI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:22:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:23:22 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "scheme.hpp"
#include "absoluteURI.hpp"

bool	isAbsoluteURI(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, ':');
	if (isScheme(str) == false)
		return (false);

	std::getline(stream, str, '?');
	if (str.length() == 0)
		return (false);
}
