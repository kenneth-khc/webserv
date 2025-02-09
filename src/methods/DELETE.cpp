/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:14:43 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/10 06:35:56 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>

// TODO: use cookies for authorization? user can only delete what is theirs
// TODO: consider other status codes. 200 OK? 202 Accepted? 403 Forbidden?
void	Server::delete_(Response& response, const Request& request) const
{
	std::string	target = request.requestTarget;

	if (target[0] == '/')
	{
		target.erase(0, 1);
	}

	if (access(target.c_str(), F_OK) == 0)
	{
		int status = std::remove(target.c_str());
		if (status == 0)
		{
			response.statusCode = 204;
			response.reasonPhrase = "No Content";
		}
		else
		{
			std::cerr << "Failed to remove " << target << '\n';
		}
	}
	else
	{
		std::cerr << "File " << target << " not found or cannot be accessed\n\n";
		response.statusCode = 404;
		response.reasonPhrase = "Not Found";
		response.messageBody = getFileContents("html/error404.html");
	}
}
