/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/06 22:31:56 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include "Server.hpp"
#include "Response.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "debugUtils.hpp"
#include "preconditions.hpp"
#include "contentType.hpp"
#include "etag.hpp"

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
		if (processIfModifiedHeader(request, statbuf.st_mtim) == false)
		{
			response.setStatusCode(Response::NOT_MODIFIED);
		}
		else if (access(file.c_str(), R_OK) == 0)
		{
			response.setStatusCode(Response::OK);
			response.messageBody = getFileContents(file);
			response.insert("Content-Length", response.messageBody.length());
			constructContentTypeHeader(file, map, response);
		}
		constructETagHeader(statbuf.st_mtim, request.find< Optional<int> >("Content-Length"));
		response.insert("Last-Modified", Time::printHTTPDate(statbuf.st_mtim));
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

