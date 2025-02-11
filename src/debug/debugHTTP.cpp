/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugHTTP.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 00:13:55 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/30 00:14:02 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debugUtils.hpp"

void	dbg::printHTTPHeaders(const std::map<std::string,std::string>& headers)
{
	std::map<std::string,std::string>::const_iterator	it;

	for (it = headers.begin(); it != headers.end(); ++it)
	{
		const std::string&	key = it->first;
		const std::string&	value = it->second;
		std::cout << key << ": " << value << '\n';
	}
}

