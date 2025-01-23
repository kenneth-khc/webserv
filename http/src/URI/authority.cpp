/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authority.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:50:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:52:21 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "authority.hpp"

bool	isAuthority(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;

	std::getline(stream, str, ':');
	if (isHost(str) == false)
		return (false);
	std::getline(stream, str);
	if (isPort(str) == false)
		return (false);
	return (true);
}