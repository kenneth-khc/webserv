/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/19 04:32:17 by cteoh            ###   ########.fr       */
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
	const Session	&session = *(response.currSession);
	std::string		file = Server::pagesDir + "/" + session["lang"].value;
	struct stat		statbuf;

	if (request.requestTarget == "/")
	{
		//TODO: try all index pages
		file += "/index.html";
	}
	else
		file += request.requestTarget;
	if (access(file.c_str(), R_OK) != 0)
		file = Server::rootDir + request.requestTarget;
	if (stat(file.c_str(), &statbuf) == 0 && access(file.c_str(), R_OK) == 0)
	{
		if (autoindex == true && S_ISDIR(statbuf.st_mode))
		{
			generateDirectoryListing(response, file);
			response.setStatusCode(Response::OK);
			response.insert("Content-Length", response.messageBody.length());
			response.insert("Content-Type", "text/html");
		}
		else if (processPreconditions(request, statbuf) == false)
		{
			response.setStatusCode(Response::NOT_MODIFIED);
		}
		else
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
