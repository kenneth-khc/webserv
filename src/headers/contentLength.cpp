/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentLength.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 05:56:12 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/03 06:34:26 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "contentLength.hpp"

bool	isContentLengthHeader(const String &line) {
	if (line.length() == 0)
		return (false);
	for (String::size_type i = 0; i < line.length(); i++) {
		if (std::isdigit(line[i]) == 0)
			return (false);
	}
	return (true);
}
