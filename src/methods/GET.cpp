/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 04:54:05 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include "Server.hpp"
#include "Response.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "Session.hpp"
#include "debugUtils.hpp"
#include "preconditions.hpp"
#include "contentType.hpp"
#include "etag.hpp"

void	Server::get(Response& response, const Request& request)
{
	std::string		file = request.requestTarget;
	const Session	&session = *(response.currSession);
	struct stat		statbuf;

	if (file == "/")
	{
		//TODO: try all index pages
		file = "html/index/" + session["lang"].value + "/index.html";
	}
	// TODO: map directories properly
	// if (file[0] == '/')
	// {
	// 	file = file.substr(1, file.npos);
	// }
	// TODO: if this is a directory, show directory listing if turned on
	// if (file[file.size()-1] == '/')
	// {
	// 	generateDirectoryListing(response, file);
	// 	return ;
	// }
	else if (access(std::string(std::string("html/index") + file).c_str(), R_OK) == 0)
		file = "html/index" + file;
	if (stat(file.c_str(), &statbuf) == 0)
	{
		if (processPreconditions(request, statbuf) == false)
		{
			response.setStatusCode(Response::NOT_MODIFIED);
		}
		else if (access(file.c_str(), R_OK) == 0)
		{
			response.setStatusCode(Response::OK);
			response.getFileContents(file);
			response.insert("Content-Length", statbuf.st_size);
			constructContentTypeHeader(file, MIMEMappings, response);
		}
		response.insert("ETag", constructETagHeader(statbuf.st_mtim, statbuf.st_size));
		response.insert("Last-Modified", Time::printHTTPDate(statbuf.st_mtim));
	}
	else
	{
		throw NotFound404();
	}
}
