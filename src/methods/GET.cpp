/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 14:56:09 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include "Response.hpp"
#include "Server.hpp"
#include "debugUtils.hpp"
#include "ErrorCode.hpp"
#include "ifModified.hpp"
#include "contentType.hpp"

void	Server::get(Response& response, const Request& request) const
{
	std::string	file = request.requestTarget;
	struct stat	statbuf;

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
	if (stat(file.c_str(), &statbuf) == 0)
	{
		if (processIfModifiedHeader(request["If-Modified-Since"].value, statbuf.st_mtim) == true)
		{
			response.setStatusCode(Response::NOT_MODIFIED);
		}
		else if (access(file.c_str(), R_OK) == 0)
		{
			response.setStatusCode(Response::OK);
			response.messageBody = getFileContents(file);
		}
		response.insert("Last-Modified", convertLastModifiedToHTTPDate(statbuf.st_mtim));
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

