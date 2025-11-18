/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 23:32:46 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/01 23:03:02 by cteoh            ###   ########.fr       */
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
#include "Server.hpp"
#include "etag.hpp"
#include <iostream>

#define FILE_NAME_LEN 45

static
Optional<String>
resolveUploadGET(Response&, const Request&, const Location&);

static
Optional<String>
resolveDirectoryGET(Response&, const Request&, const Location&);

static
void
getFile(const String&, Response&, const Request&, const Location&);

static void		generateUploadsListing(const String& uploadsDir, Response& response, const Request& request);
static String	getUploadsReference(const String& uploadsDir, const Request &request);
static void		generateDirectoryListing(Response& response, std::string dirName, const std::string& uri);
static bool		fileIsDirectory(const String& filepath);
static Optional<String>	tryIndexFiles(const Location&, const String& filepath);

void	Server::get(Response& response, Request& request, const Location& location) const
{
	String	resolvedFilepath;
	if (location.acceptUploads)
	{
		Optional<String>	uploadedFilepath = resolveUploadGET(response, request, location);
		if (uploadedFilepath.exists)
		{
			resolvedFilepath = uploadedFilepath.value;
		}
		else
		{
			return;
		}
	}
	else if (request.path.back() == '/')
	{
		Optional<String>	indexFilepath = resolveDirectoryGET(response, request, location);
		if (indexFilepath.exists)
		{
			resolvedFilepath = indexFilepath.value;
		}
		else
		{
			return;
		}
	}
	else
	{
		resolvedFilepath = request.resolvedPath;
	}

	getFile(resolvedFilepath, response, request, location);
}

static Optional<String>	resolveUploadGET(Response& response, 
		const Request& request, const Location& location)
{
	if (request.path == location.uri && fileIsDirectory(request.resolvedPath))
	{
		generateUploadsListing("uploads", response, request);
		response.setStatusCode(Response::OK);
		response.insert("Content-Length", response.messageBody.length());
		response.insert("Content-Type", "text/html");
		return makeNone<String>();
	}
	else
	{
		String	uploadedFilepath = getUploadsReference("uploads", request);
		return makeOptional(uploadedFilepath);
	}
}

static Optional<String>	resolveDirectoryGET(Response &response,
		const Request& request, const Location& location)
{
	Optional<String>	indexFilepath = tryIndexFiles(location, request.resolvedPath);
	if (!indexFilepath.exists)
	{
		if (location.autoindex)
		{
			generateDirectoryListing(response, request.resolvedPath, location.uri);
			response.setStatusCode(Response::OK);
			response.insert("Content-Length", response.messageBody.length());
			response.insert("Content-Type", "text/html");
			return makeNone<String>();
		}
		else
		{
			return makeOptional(location.indexFiles.back());
		}
	}
	else
	{
		return makeOptional(indexFilepath.value);
	}
}

static bool	fileIsDirectory(const String& filepath)
{
	struct stat	filestatus;

	if (stat(filepath.c_str(), &filestatus) == -1)
	{
		// TODO: wat to do in case of errors?
		std::cerr << "stat(" << filepath << ") failed\n";
		return false;
	}
	else
	{
		return S_ISDIR(filestatus.st_mode);
	}
}

static Optional<String>	tryIndexFiles(const Location& location, const String& filepath)
{
	size_t	i;
	for (i = 0; i < location.indexFiles.size(); ++i)
	{
		const String&	indexFile = filepath + location.indexFiles[i];
		if (access(indexFile.c_str(), R_OK) == 0)
		{
			return makeOptional(indexFile);
		}
	}
	return makeNone<String>();
}

void	getFile(const String& filepath, Response& response,
				const Request& request, const Location& location)
{
	struct stat	status;
	if (stat(filepath.c_str(), &status) == 0 && access(filepath.c_str(), R_OK) == 0)
	{
		if (processPreconditions(request, status) == false)
		{
			response.setStatusCode(Response::NOT_MODIFIED);
		}
		else
		{
			response.setStatusCode(Response::OK);
			response.getFileContents(filepath);
			response.insert("Content-Length", status.st_size);
			constructContentTypeHeader(response, filepath, location.MIMEMappings);
		}
		response.insert("ETag", constructETagHeader(status.st_mtim, status.st_size));
		response.insert("Last-Modified", Time::printHTTPDate(status.st_mtim));
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

#include <iomanip>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>

#define FILE_NAME_LEN 45
#define FILE_SIZE_LEN 20
#include <iostream>

void	generateDirectoryListing(Response& response,
    	                         std::string path,
    	                         const std::string& uri)
{
	DIR*	dir = opendir(path.c_str());

	if (!dir)
		throw NotFound404();

	struct stat			statbuf;
	std::stringstream	stream;

	if (path[path.length() - 1] != '/')
		path += "/";

	stream << "<html>\n"
		   << 	"<head>\n"
		   << 		"<title>Index of " + uri + "</title>\n"
		   << 		"<style>\n\t"
		   << 			"html { font-family: 'Comic Sans MS' }\n\t"
		   << 			"body { background-color: #f4dde7 }\n\t"
		   << 			"p { display: inline }\n"
		   << 		"</style>\n"
		   << 	"</head>\n"
		   << 	"<body>\n"
		   << 		"<h1>Index of " + uri + "</h1>\n"
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
			   << ((entry->d_type == DT_DIR) ? uri + d_name + "/" : uri + d_name)
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
	closedir(dir);
}
