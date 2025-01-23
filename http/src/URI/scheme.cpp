/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheme.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:03:36 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:03:53 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scheme.hpp"

bool	isScheme(const std::string &line) {
	static const std::string	values = "+-.";

	if (std::isalpha(line[0]) == 0)
		return (false);
	
	for (int i = 1; i < line.length(); i++) {
		if (std::isalpha(line[i]) != 0)
			continue ;
		if (std::isdigit(line[i]) != 0)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);		
	}
	return (true);
}
