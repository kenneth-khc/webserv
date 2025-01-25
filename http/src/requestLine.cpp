/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:13:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 18:41:21 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "requestTarget.hpp"
#include "requestLine.hpp"

bool	isRequestLine(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, ' ');
	if (isToken(str) == false)
		return (false);
	std::getline(stream, str, ' ');
	if (isRequestTarget(str) == false)
		return (false);
	std::getline(stream, str, '/');
	if (str != "HTTP")
		return (false);
	std::getline(stream, str);
	if (str.length() != 3)
		return (false);
	if (std::isdigit(str[0]) == 0 || str[1] != '.' || std::isdigit(str[2]) == 0)
		return (false);
	return (true);
}
