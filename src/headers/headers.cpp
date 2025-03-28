/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:47:44 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/03 05:27:26 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uri.hpp"
#include "headers.hpp"

bool	checkMandatoryHeaders(String fieldName, const String &fieldValue) {
	const String	headers[NUM_OF_MANDATORY_HEADERS] = {
		"host"
	};
	bool	(*const funcs[NUM_OF_MANDATORY_HEADERS])(const String &) = {
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