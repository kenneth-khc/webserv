/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authority.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:50:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 18:27:40 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "host.hpp"
#include "port.hpp"
#include "authority.hpp"

bool	isAuthority(const std::string &line) {
	if (line.find(':') == std::string::npos)
		return (false);

	std::stringstream	stream(line);
	std::string			str;
	std::getline(stream, str, ':');
	if (isHost(str) == false)
		return (false);
	if (!std::getline(stream, str))
		return (true);
	if (isPort(str) == false)
		return (false);
	return (true);
}