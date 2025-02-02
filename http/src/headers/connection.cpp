/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/04 03:57:19 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "connection.hpp"

void	processConnectionHeader(const Request &request, Response &response) {
	std::string value = request["Connection"];

	for (std::size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);
	if (value.find("keep-alive")) {
		response.insert("Connection", "keep-alive");
		response.flags |= Response::CONNECTION;
	}
	else
		response.insert("Connection", "close");
}
