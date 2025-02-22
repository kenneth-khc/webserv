/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:47:44 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 03:12:55 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uri.hpp"
#include "headers.hpp"

bool	checkMandatoryHeaders(
std::string fieldName,
const std::string &fieldValue)
{
	static const std::string	headers[NUM_OF_MANDATORY_HEADERS] = {
		"host"
	};
	static bool					(*const funcs[NUM_OF_MANDATORY_HEADERS])(const std::string &) = {
		isAuthority
	};

	for (int i = 0; i < NUM_OF_MANDATORY_HEADERS; i++) {
		if (fieldName == headers[i]) {
			if (funcs[i](fieldValue) == false)
				return (false);
		}
	}
	return (true);
}