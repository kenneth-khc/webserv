/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:14:43 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/20 00:31:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include "String.hpp"
#include "ErrorCode.hpp"
#include "Server.hpp"
#include "Driver.hpp"

// TODO: consider other status codes. 200 OK? 202 Accepted? 403 Forbidden?
void	Driver::delete_(Request& request, Response& response) const
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

	while (entry != 0)
	{
		String	d_name(entry->d_name);

		if (d_name.find(sid).exists == true && d_name.find(fileName).exists == true)
		{
			std::remove((uploadsDir + "/" + d_name).c_str());
			break ;
		}
		entry = readdir(dir);
	}
	if (entry == 0)
	{
		throw NotFound404();
	}

	response.setStatusCode(Response::OK);
	response.insert("Content-Type", "text/html");
	response.insert("Content-Length", 0);
	response.processStage = Response::POST_PROCESSING;
}
