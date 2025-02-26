/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:14:43 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/26 21:23:25 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include "String.hpp"
#include "ErrorCode.hpp"
#include "Server.hpp"

// TODO: use cookies for authorization? user can only delete what is theirs
// TODO: consider other status codes. 200 OK? 202 Accepted? 403 Forbidden?
void	Server::delete_(Response& response, const Request& request) const
{
	DIR*	dir = opendir(uploadsDir.c_str());

	if (!dir)
	{
		throw NotFound404();
	}

	Optional<String::size_type>	uploads = request.requestTarget.find("/" + uploadsDir);

	if (uploads.exists == false || uploads.value != 0)
	{
		throw NotFound404();
	}

	Optional<String::size_type>	pos = request.requestTarget.find("/", 1);
	String						fileName = request.requestTarget.substr(pos.value + 1);
	String						sid = request.cookies.find("sid")->second.value;
	dirent*						entry = readdir(dir);

	while (entry != NULL)
	{
		String	d_name(entry->d_name);

		if (d_name.find(sid).exists == true && d_name.find(fileName).exists == true)
		{
			std::remove((uploadsDir + "/" + d_name).c_str());
			break ;
		}
		entry = readdir(dir);
	}
	if (entry == NULL)
	{
		throw NotFound404();
	}

	std::ifstream	ifs((miscPagesDir + "/delete_success.html").c_str());

	response.setStatusCode(Response::OK);
	String::getline(ifs, response.messageBody, '\0');
	response.insert("Content-Type", "text/html");
	response.insert("Content-Length", response.messageBody.length());
}
