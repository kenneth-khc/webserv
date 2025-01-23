/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:10:35 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:11:30 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path.hpp"
#include "query.hpp"

bool	isQuery(const std::string &line) {
	static const std::string	values = "/?";

	for (int i = 0; i < line.length(); i++) {
		if (isPrintableCharacter(line, i) == true)
			continue ;
		if (values.find(line[i]) != std::string::npos)
			continue ;
		return (false);		
	}
	return (true);
}
