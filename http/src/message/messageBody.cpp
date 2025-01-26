/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:00:09 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 01:18:34 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "messageBody.hpp"

bool	isMessageBody(const std::string &line) {
	for (std::size_t i = 0; i < line.length(); i++) {
		if (static_cast<std::size_t>(line[i]) > 255)
			return (false);
	}
	return (true);
}
