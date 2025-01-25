/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:34:46 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 21:41:11 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Request.hpp"

const std::string	Request::methods[NUM_OF_METHODS] = { "GET", "HEAD", "POST", "PUT", "DELETE" };

static void	extractRequestLine(Request &obj, const std::string &message) {
	std::stringstream	messageStream(message);
	std::string			input;

	std::getline(messageStream, input, ' ');
	for (std::size_t i = 0; i <= NUM_OF_METHODS; i++) {
		// if (i == NUM_OF_METHODS)
		// 	throw something;
		if (input == Request::methods[i]) {
			obj.method = i;
			break ;
		}
	}
	
	std::getline(messageStream, input, ' ');

}

Request::Request(const std::string &message) {
	
}