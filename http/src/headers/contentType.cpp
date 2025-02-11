/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:09:40 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 16:40:53 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Optional.hpp"
#include "contentType.hpp"

void	constructContentTypeHeader(
const std::string &file,
const MediaType &map,
Response &response) {
	std::size_t	pos = file.find('.');
	std::string	extension = file.substr(pos + 1);

	Optional<std::string>	mediaType = map[extension];

	if (mediaType.exists == false)
		response.insert("Content-Type", "application/octet-stream");
	else
		response.insert("Content-Type", mediaType.value);
}
