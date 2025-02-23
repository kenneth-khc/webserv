/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 22:30:51 by cteoh            ###   ########.fr       */
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
#include "Server.hpp"
#include "Response.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "debugUtils.hpp"
#include "preconditions.hpp"
#include "contentType.hpp"
#include "etag.hpp"

#define FILE_NAME_LEN 45

static void		generateUploadsListing(Response& response, const Request& request);
static String	getUploadsReference(const Request &request);

void	Server::get(Response& response, const Request& request)
{
	if (request.requestTarget == ("/" + Server::uploadsDir))
	{
		if (request.method == Request::GET)
		{
			generateUploadsListing(response, request);
		}
		response.setStatusCode(Response::OK);
		response.insert("Content-Length", response.messageBody.length());
		response.insert("Content-Type", "text/html");
		return ;
	}

	Optional<String::size_type>	uploads = request.requestTarget.find("/" + Server::uploadsDir);
	const String&				lang = request.cookies.find("lang")->second.value;
	String						file;
	struct 	stat				statbuf;

	if (uploads.exists == true && uploads.value == 0)
	{
		file = getUploadsReference(request);
	}
	else
	{
		file = Server::rootDir + "/" + Server::pagesDir + "/" + lang;
		if (request.requestTarget == "/")
		{
			//TODO: try all index pages
			file += "/index.html";
		}
		else
		{
			file += request.requestTarget;
		}
	}

	if (access(file.c_str(), R_OK) != 0)
		file = Server::rootDir + request.requestTarget;
	if (stat(file.c_str(), &statbuf) == 0 && access(file.c_str(), R_OK) == 0)
	{
		if (autoindex == true && S_ISDIR(statbuf.st_mode))
		{
			if (request.method == Request::GET)
			{
				generateDirectoryListing(response, file);
			}
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
			if (request.method == Request::GET)
			{
				response.getFileContents(file);
			}
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

static void	generateUploadsListing(Response& response, const Request& request)
{
	DIR*	dir = opendir(Server::uploadsDir.c_str());

	if (!dir)
		throw NotFound404();

	std::stringstream	stream;
	std::vector<String>	uploadsList;

	stream << "<html>\n"
		   << 	"<head>\n"
		   << 		"<style>\n\t"
		   <<			"html { font-family: 'Comic Sans MS' }\n\t"
		   << 			"body { background-color: #f4dde7 }\n\t"
		   << 		"</style>\n"
		   << 	"</head>\n"
		   <<	"<body>\n"
		   <<		"<h1>Uploads</h1>\n"
		   <<		"<hr><pre>\n";

	response.messageBody += stream.str();

	const String&	sid = request.cookies.find("sid")->second.value;
	for (dirent* entry = readdir(dir); entry != NULL; entry = readdir(dir))
	{
		String						file(entry->d_name);
		Optional<String::size_type> sidMatch = file.find(sid);

		if (sidMatch.exists == false || sidMatch.value != 0)
			continue ;

		Optional<String::size_type>	delimiter = file.find("_");
		String						trim = file.substr(delimiter.value + 1);
		String						truncate = trim;

		if (truncate.length() > FILE_NAME_LEN)
		{
			truncate.resize(FILE_NAME_LEN);
			truncate.replace(FILE_NAME_LEN - 3, 3,"...");
		}
		stream.str("");
		stream << "<a href=\""
			   << "/" + Server::uploadsDir + "/" + trim
			   << "\">"
			   << std::left
			   << std::setw(FILE_NAME_LEN + 5)
			   << truncate + "</a> "
			   << "<button type=\"button\" onclick=\"del("
			   << "'/" + Server::uploadsDir + "/" + trim + "'"
			   << ")\">X</button>\n";
		uploadsList.push_back(stream.str());
	}

	std::sort(uploadsList.begin(), uploadsList.end());

	for (std::vector<String>::const_iterator it = uploadsList.begin(); it != uploadsList.end(); it++)
	{
		response.messageBody += *it;
	}

	stream.str("");
	stream << 		"</pre><hr>\n"
		   << 		"<script>"
		   <<			"async function del(url) { await fetch(url, { method: \"DELETE\" }); }"
		   <<		"</script>"
		   << 	"</body>\n"
		   << "</html>";
	response.messageBody += stream.str();
}

static String	getUploadsReference(const Request &request)
{
	DIR*	dir = opendir(Server::uploadsDir.c_str());

	if (!dir)
	{
		throw NotFound404();
	}

	Optional<String::size_type>	pos = request.requestTarget.find("/", 1);
	String						fileName = request.requestTarget.substr(pos.value + 1);
	String						sid = request.cookies.find("sid")->second.value;

	for (dirent* entry = readdir(dir); entry != NULL; entry = readdir(dir))
	{
		String	d_name(entry->d_name);

		if (d_name.find(sid).exists == true && d_name.find(fileName).exists == true)
		{
			return (Server::uploadsDir + "/" + d_name);
		}
	}
	throw NotFound404();
}
