/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:11:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 00:10:03 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Request.hpp"

const std::string	Request::methods[NUM_OF_METHODS] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"OPTIONS",
	"DELETE"
};

bool	Request::isValidMethod(const std::string &method) {
	for (int i = 0; i < NUM_OF_METHODS; i++) {
		if (method == methods[i]) {
			this->method = i;
			return (true);
		}
	}
	return (false);
}

Request::Request() {}

Request::~Request() {}
