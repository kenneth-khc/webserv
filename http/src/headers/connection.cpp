/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 14:45:53 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Optional.hpp"
#include "connection.hpp"

void	processConnectionHeader(const Request &request, Response &response) {
	Optional<std::string> connectionOption = request["Connection"];

	for (std::size_t i = 0; i < connectionOption.value.length(); i++)
		connectionOption.value[i] = std::tolower(connectionOption.value[i]);
	if (connectionOption.value.find("keep-alive")) {
		response.insert("Connection", "keep-alive");
		response.flags |= Response::CONNECTION;
	}
	else
		response.insert("Connection", "close");
}
