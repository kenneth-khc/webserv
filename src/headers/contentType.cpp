/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:09:40 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/15 22:15:03 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Optional.hpp"
#include "contentType.hpp"

void	constructContentTypeHeader(
const String &file,
const MediaType &MIMEMappings,
Response &response) {
	Optional<String::size_type>	pos = file.find('.');
	String						extension = file.substr(pos.value + 1);
	Optional<String>			mediaType = MIMEMappings[extension];

	if (mediaType.exists == false)
		response.insert("Content-Type", "application/octet-stream");
	else
		response.insert("Content-Type", mediaType.value);
}
