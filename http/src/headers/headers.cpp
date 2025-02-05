/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:47:44 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 18:23:19 by cteoh            ###   ########.fr       */
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
	static bool					(*const checkFunctions[NUM_OF_MANDATORY_HEADERS])(const std::string &) = {
		isAuthority
	};

	for (std::size_t i = 0; i < fieldName.length(); i++)
		fieldName[i] = std::tolower(fieldName[i]);

	int i = 0;
	while (i < NUM_OF_MANDATORY_HEADERS) {
		if (fieldName == headers[i])
			break ;
		i++;
	}
	if (i == NUM_OF_MANDATORY_HEADERS)
		return (true);
	return (checkFunctions[i](fieldValue));
}