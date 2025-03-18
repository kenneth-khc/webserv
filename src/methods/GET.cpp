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
#include <iomanip>
#include <vector>
#include "String.hpp"
#include "Optional.hpp"
#include "ErrorCode.hpp"
#include "Time.hpp"
#include "preconditions.hpp"
#include "contentType.hpp"
#include "etag.hpp"

#include "Driver.hpp"

#define FILE_NAME_LEN 45
static void		generateUploadsListing(const String& uploadsDir, Response& response, const Request& request);
static String	getUploadsReference(const String& uploadsDir, const Request &request);
static void		getFromFileSystem(Response& response, const Request& request, const Location&);
static void		generateDirectoryListing(Response& response, const std::string& dirName);

void	Server::get(Response& response, Request& request, const Location& location) const
{
	// TODO: cgi dir
	Optional<String::size_type>	pos = request.path.find(String("/") + "cgi-bin" + "/");

	if (pos.exists == true && pos.value == 0)
	{
		cgi(response, request);
		return ;
	}
	// TODO: uploads dir
	else if (request.path == (String("/") + "uploads"))
	{
		if (request.method == "GET")
		{
			generateUploadsListing("uploads", response, request);
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

	getFromFileSystem(response, request, location);
}

static void		getFromFileSystem(Response& response, const Request& request,
								  const Location& location)
{
	Optional<String::size_type>	uploads = request.path.find(String("/") + "uploads");
	String						filePath = request.resolvedPath;
	struct 	stat				statbuf;
	if (uploads.exists == true && uploads.value == 0)
	{
		filePath = getUploadsReference("uploads", request);
	}
	else
	{
		if (request.path == "/")
		{
			//TODO: try all index pages
			filePath += "/index.html";
			response.insert("Cache-Control", "no-cache");
		}
		else
		{
			filePath = request.resolvedPath;
		}
	}

	if (stat(filePath.c_str(), &statbuf) == 0 && access(filePath.c_str(), R_OK) == 0)
	{
		if (location.autoindex == true && S_ISDIR(statbuf.st_mode))
		{
			if (request.method == "GET")
			{
				generateDirectoryListing(response, filePath);
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
				response.getFileContents(filePath);
			}
			response.insert("Content-Length", statbuf.st_size);
			constructContentTypeHeader(filePath, location.MIMEMappings, response);
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

#include <iomanip>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>

#define FILE_NAME_LEN 45
#define FILE_SIZE_LEN 20

void	generateDirectoryListing(Response& response, const std::string& dirName)
{
	DIR*	dir = opendir(dirName.c_str());

	if (!dir)
		throw NotFound404();

	struct stat			statbuf;
	std::string			path = dirName;
	std::string			trimmedRootPath;
	std::stringstream	stream;

	if (path[path.length() - 1] != '/')
		path += "/";

	trimmedRootPath = path.substr(path.find_first_of('/'));
	stream << "<html>\n"
		   << 	"<head>\n"
		   << 		"<title>Index of " + trimmedRootPath + "</title>\n"
		   << 		"<style>\n\t"
		   << 			"html { font-family: 'Comic Sans MS' }\n\t"
		   << 			"body { background-color: #f4dde7 }\n\t"
		   << 			"p { display: inline }\n"
		   << 		"</style>\n"
		   << 	"</head>\n"
		   << 	"<body>\n"
		   << 		"<h1>Index of " + trimmedRootPath + "</h1>\n"
		   << 		"<hr><pre>\n";

	response.messageBody += stream.str();

	std::string					parentDir;
	std::vector<std::string>	directories;
	std::vector<std::string>	regularFiles;

	for (dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
	{
		std::string	d_name(entry->d_name);
		std::string	str;
		std::string	truncate;

		if (d_name == ".")
		{
			continue ;
		}

		stream.str("");
		// File/Directory Name
		truncate = (entry->d_type == DT_DIR) ? d_name + "/" : d_name;
		if (truncate.length() > FILE_NAME_LEN)
		{
			truncate.resize(FILE_NAME_LEN);
			if (entry->d_type == DT_DIR)
			{
				truncate.replace(FILE_NAME_LEN - 4, 4, ".../");
			}
			else
			{
				truncate.replace(FILE_NAME_LEN - 3, 3, "...");
			}
		}

		stream << "<a href=\""
			   << ((entry->d_type == DT_DIR) ? trimmedRootPath + d_name + "/" : trimmedRootPath + d_name)
			   << "\">"
			   << std::left
			   << std::setw(FILE_NAME_LEN + 5)
			   << truncate + "</a> ";

		str += stream.str();
		if (d_name == "..")
		{
			parentDir = str + "\n";
			continue ;
		}

		// Last Modified Date and File Size in Bytes
		std::stringstream	streamTwo;

		stream.str("");
		stat(path.c_str(), &statbuf);
		stream << "<p>";
		if (entry->d_type == DT_DIR)
		{
			streamTwo << "-";
		}
		else
		{
			streamTwo << statbuf.st_size;
		}
		stream << Time::printAutoindexDate(statbuf.st_mtim)
			   << " "
			   << std::right
			   << std::setw(FILE_SIZE_LEN)
			   << streamTwo.str()
			   << "</p>\n";

		str += stream.str();
		if (entry->d_type == DT_DIR)
			directories.push_back(str);
		else
			regularFiles.push_back(str);
	}

	std::sort(directories.begin(), directories.end());
	std::sort(regularFiles.begin(), regularFiles.end());

	response.messageBody += parentDir;
	for (std::vector<std::string>::const_iterator it = directories.begin(); it != directories.end(); it++)
	{
		response.messageBody += *it;
	}
	for (std::vector<std::string>::const_iterator it = regularFiles.begin(); it != regularFiles.end(); it++)
	{
		response.messageBody += *it;
	}

	stream.str("");
	stream << 		"</pre><hr>\n"
		   << 	"</body>\n"
		   << "</html>";
	response.messageBody += stream.str();
	closedir(dir);
}
