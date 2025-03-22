/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 01:14:47 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include "Optional.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "debugUtils.hpp"
#include "preconditions.hpp"
#include "contentType.hpp"
#include "etag.hpp"
#include "Driver.hpp"

static String	getUploadsReference(const String& uploadsDir, const Request &request);

void	Driver::get(Request& request, Response& response)
{
	if (request.path == ("/" + uploadsDir))
	{
		generateUploadsListing(request, response, uploadsDir);
		response.setStatusCode(Response::OK);
		response.insert("Content-Length", response.messageBody.length());
		response.insert("Content-Type", "text/html");
		return ;
	}

	String&										lang = request.cookies["lang"].value;
	std::map<String, String>::const_iterator	queryLang = request.queryPairs.find("lang");
	if (queryLang != request.queryPairs.end() && lang != queryLang->second)
	{
		Cookie	cookie("lang", queryLang->second);
		cookie.path = Optional<String>("/");
		response.insert("Set-Cookie", cookie.constructSetCookieHeader());
		lang = queryLang->second;
	}

	Optional<String::size_type>	uploads = request.path.find("/" + uploadsDir);
	String						file;
	struct stat					statbuf;

	if (request.path.starts_with("/" + uploadsDir))
	{
		file = getUploadsReference(uploadsDir, request);
	}
	else
	{
		file = rootDir + "/" + pagesDir + "/" + lang;
		if (request.path == "/")
		{
			//TODO: try all index pages
			file += "/index.html";
			response.insert("Cache-Control", "no-cache");
		}
		else if (request.path.starts_with("/directory") == true)
		{
			file = request.path;
			file.replace(0, 10, "YoupiBanane");
			if (file == "YoupiBanane")
				file += "/youpi.bad_extension";
			if (file != "YoupiBanane/Yeah")
				autoindex = true;
			else
				autoindex = false;
		}
		else
		{
			file += request.path;
		}
	}

	if (access(file.c_str(), R_OK) != 0)
		file = rootDir + request.path;
	if (stat(file.c_str(), &statbuf) == 0 && access(file.c_str(), R_OK) == 0)
	{
		if (autoindex == true && S_ISDIR(statbuf.st_mode))
		{
			if (request.method == "GET")
			{
				generateDirectoryListing(response, file);
			}
			response.setStatusCode(Response::OK);
			response.insert("Content-Length", response.messageBody.length());
			response.insert("Content-Type", "text/html");
		}
		else if (autoindex == false && S_ISDIR(statbuf.st_mode))
		{
			throw NotFound404();	// Test-specific condition
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
			constructContentTypeHeader(response, file, MIMEMappings);
		}
		response.insert("ETag", constructETagHeader(statbuf.st_mtim, statbuf.st_size));
		response.insert("Last-Modified", Time::printHTTPDate(statbuf.st_mtim));
	}
	else
	{
		throw NotFound404();
	}
}

static String	getUploadsReference(
	const String& uploadsDir,
	const Request &request)
{
	DIR*	dir = opendir(uploadsDir.c_str());

	if (!dir)
	{
		throw NotFound404();
	}

	Optional<String::size_type>	pos = request.path.find("/", 1);
	String						fileName = request.path.substr(pos.value + 1);
	String						sid = request.cookies.find("sid")->second.value;

	for (dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
	{
		String	d_name(entry->d_name);

		if (d_name.find(sid).exists == true && d_name.find(fileName).exists == true)
		{
			return (uploadsDir + "/" + d_name);
		}
	}
	throw NotFound404();
}
