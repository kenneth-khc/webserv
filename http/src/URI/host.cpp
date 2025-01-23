/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:49:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 00:54:20 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "host.hpp"

bool	isIPV6(const std::string &line) {
	std::stringstream	stream(line);
	std::string			str;
	int					counter = 0;

	while(std::getline(stream, str, ':')) {
		if (str.length() == 0)
			continue ;
		if 
	}
}

bool	isIPLiteral(const std::string &line) {
	if (line[0] != '[' || line[line.length() - 1] != ']')
		return (false);

	std::string	str = line.substr(1, line.length() - 1);
	if (isIPV6(str) == true)
		return (true);
	if (isIPVFuture(str) == true)
		return (true);
	return (false);
}

bool	isHost(const std::string &line) {
	
}