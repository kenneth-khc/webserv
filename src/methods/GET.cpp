/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/08 16:46:28 by cteoh            ###   ########.fr       */
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
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "debugUtils.hpp"
#include "preconditions.hpp"
#include "contentType.hpp"
#include "etag.hpp"

#define FILE_NAME_LEN 45

static void		generateUploadsListing(const String& uploadsDir, Response& response, const Request& request);
static String	getUploadsReference(const String& uploadsDir, const Request &request);

void	Server::get(Response& response, Request& request) const
{
	Optional<String::size_type>	pos = request.absolutePath.find("/" + cgiDir + "/");

	if (pos.exists == true && pos.value == 0)
	{
		cgi(response, request);
		return ;
	}
	else if (request.absolutePath == ("/" + uploadsDir))
	{
		if (request.method == "GET")
		{
			generateUploadsListing(uploadsDir, response, request);
		}
		response.setStatusCode(Response::OK);
		response.insert("Content-Length", response.messageBody.length());
		response.insert("Content-Type", "text/html");
		return ;
	}

	String&										lang = request.cookies["lang"].value;
	std::map<String, String>::const_iterator	queryLang = request.queryPairs.find("lang");
	if (queryLang != request.queryPairs.end() && lang != queryLang->second)
	{
		response.insert("Set-Cookie", "lang=" + queryLang->second);
		lang = queryLang->second;
	}

	Optional<String::size_type>	uploads = request.absolutePath.find("/" + uploadsDir);
	String						file;
	struct 	stat				statbuf;

	if (uploads.exists == true && uploads.value == 0)
	{
		file = getUploadsReference(uploadsDir, request);
	}
	else
	{
		file = rootDir + "/" + pagesDir + "/" + lang;
		if (request.absolutePath == "/")
		{
			//TODO: try all index pages
			file += "/index.html";
			response.insert("Cache-Control", "no-cache");
		}
		else
		{
			file += request.absolutePath;
		}
	}

	if (access(file.c_str(), R_OK) != 0)
		file = rootDir + request.absolutePath;
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
		else if (processPreconditions(request, statbuf) == false)
		{
			response.setStatusCode(Response::NOT_MODIFIED);
		}
		else
		{
			response.setStatusCode(Response::OK);
			if (request.method == "GET")
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

static void	generateUploadsListing(
	const String& uploadsDir,
	Response& response,
	const Request& request)
{
	DIR*	dir = opendir(uploadsDir.c_str());

	if (!dir)
		throw NotFound404();

	std::stringstream	stream;
	std::vector<String>	uploadsList;

	stream << "<html>\n"
		   << 	"<head>\n"
		   << 		"<style>\n\t"
		   <<			"html { font-family: 'Comic Sans MS' }\n\t"
		   << 			"body { background-color: #f4dde7 }\n"
		   << 		"</style>\n"
		   << 	"</head>\n"
		   <<	"<body>\n"
		   <<		"<h1>Uploads</h1>\n"
		   <<		"<hr><pre>\n";

	response.messageBody += stream.str();

	const String&	sid = request.cookies.find("sid")->second.value;
	for (dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
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
			   << "/" + uploadsDir + "/" + trim
			   << "\">"
			   << std::left
			   << std::setw(FILE_NAME_LEN + 5)
			   << truncate + "</a> "
			   << "<button type=\"button\" onclick=\"del("
			   << "'/" + uploadsDir + "/" + trim + "'"
			   << ")\">Delete</button>\n";
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

static String	getUploadsReference(
	const String& uploadsDir,
	const Request &request)
{
	DIR*	dir = opendir(uploadsDir.c_str());

	if (!dir)
	{
		throw NotFound404();
	}

	Optional<String::size_type>	pos = request.absolutePath.find("/", 1);
	String						fileName = request.absolutePath.substr(pos.value + 1);
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
