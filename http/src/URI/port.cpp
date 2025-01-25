/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   port.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:12:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 21:41:11 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "port.hpp"

bool	isPort(const std::string &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if (std::isdigit(line[i]) == 0)
			return (false);
	}
	return (true);
}
