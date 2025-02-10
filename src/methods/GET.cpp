/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/10 08:07:41 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fstream>
#include "Response.hpp"
#include "Server.hpp"
#include "debugUtils.hpp"
#include "ErrorCode.hpp"

void	Server::get(Response& response, const Request& request) const
{
	std::string	file = request.requestTarget;

	if (file == "/")
	{
		//TODO: try all index pages
		file = "html/index.html";
	}
	// TODO: map directories properly
	if (file[0] == '/')
	{
		file = file.substr(1, file.npos);
	}
	// TODO: if this is a directory, show directory listing if turned on
	if (file[file.size()-1] == '/')
	{
		generateDirectoryListing(response, file);
		return ;
	}
	if (access(file.c_str(), F_OK) == 0)
	{
		if (access(file.c_str(), R_OK) == 0)
		{
			response.statusCode = 200;
			response.reasonPhrase = "OK";
			response.messageBody = getFileContents(file);
			response.headers.insert(std::make_pair("Connection", "close"));
		}
	}
	else
	{
		response.messageBody = getFileContents("html/error404.html");
		throw NotFound404();
	}
}

std::string	Server::getFileContents(const std::string& file) const
{
	std::ifstream	filestream(file.c_str());
	std::string		fileContents;
	std::string		str;

	while (std::getline(filestream, str))
	{
		fileContents += str;
		fileContents += "\n";
	}
	return fileContents;
}

