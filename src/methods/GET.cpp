/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/28 00:58:30 by kecheong         ###   ########.fr       */
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
	if (access(file.c_str(), F_OK) == 0)
	{
		if (access(file.c_str(), R_OK) == 0)
		{
			response.statusCode = 200;
			response.reasonPhrase = "OK";
			response.body = getFileContents(file);
		}
	}
	else
	{
		response.body = getFileContents("html/error404.html");
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

