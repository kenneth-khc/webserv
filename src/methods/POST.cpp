/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 04:33:58 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/21 06:08:00 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <map>
#include "String.hpp"
#include "Optional.hpp"
#include "Time.hpp"
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "POSTBody.hpp"

using std::size_t;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::map;

static String	constructFilePath(const String& uploadsDir, const String& sid, const String& fileName);
static void		uploadFiles(const POSTBody& body, const String& uploadsDir, const String& sid);
static String	constructFormPath(const String& uploadsDir, const String& sid);
static void		uploadForm(const POSTBody& body, const String& uploadsDir, const String& sid);

/*
	Allows uploading HTML forms and files. Makes use of the cookies set
	automatically by the server to differentiate the uploaded resources.
*/
void	Server::post(Response& response, const Request& request) const
{
	POSTBody	msgBody(request);

	if (request.location->acceptUploads)
	{
		POSTBody		msgBody(request);
		const String&	sid = request.cookies.find("sid")->second.value;

		//TODO: dynamic uploads dir
		if (msgBody.contentType == "application/x-www-form-urlencoded")
		{
			uploadForm(msgBody, request.location->uploadDirectory, sid);
		}
		else if (msgBody.contentType == "multipart/form-data")
		{
			uploadFiles(msgBody, request.location->uploadDirectory, sid);
		}
		else
		{
			throw UnsupportedMediaType415("Accept", "application/x-www-form-urlencoded, multipart/form-data");
		}
		response.setStatusCode(Response::SEE_OTHER);
		response.insert("Content-Length", 0);
		response.insert("Location", "/");
	}
	else
	{
		throw NotFound404();
	}
}

static String	constructFilePath(const String& uploadsDir,
								  const String& sid,
								  const String& fileName)
{
	Optional<String::size_type>	pos = fileName.find_last_of('.');
	String						tempFilePath;
	size_t						i = 0;
	String						extension;
	stringstream				stream;
	String						filePath;

	if (pos.exists == true)
	{
		extension = fileName.substr(pos.value);
		tempFilePath = uploadsDir + "/" + sid + "_" + fileName.substr(0, pos.value);
	}
	else
		tempFilePath = uploadsDir + "/" + sid + "_" + fileName;
	while (i < String::npos)
	{
		if (i != 0)
		{
			stream << i;
			filePath = tempFilePath + stream.str() + extension;
			stream.str("");
		}
		else
		{
			filePath = tempFilePath + extension;
		}
		if (access(filePath.c_str(), F_OK) != 0)
		{
			break ;
		}
		i++;
	}
	if (i == String::npos)
		filePath = tempFilePath + extension;
	return (filePath);
}

static void	uploadFiles(const POSTBody& body, const String& uploadsDir, const String& sid)
{
	String		uploadDest;
	ofstream	outfile;


	for (vector<POSTBodyPart>::const_iterator it = body.parts.begin();
		 it != body.parts.end(); ++it)
	{
		map<String,String>::const_iterator fname = it->contentDisposition.find("filename");
		if (fname != it->contentDisposition.end())
		{
			uploadDest = constructFilePath(uploadsDir, sid, fname->second);
			outfile.open(uploadDest.c_str());
			if (outfile)
			{
				outfile << it->content;
			}
			else
			{
				throw InternalServerError500();
			}
			outfile.close();
		}
	}
}

static String	constructFormPath(const String& uploadsDir, const String& sid)
{
	String			tempFormPath = uploadsDir + "/" + sid + "_form";
	size_t			i = 0;
	stringstream	stream;
	String			formPath;

	while (i < String::npos)
	{
		if (i != 0)
		{
			stream << i;
			formPath = tempFormPath + stream.str() + ".json";
			stream.str("");
		}
		else
		{
			formPath = tempFormPath + ".json";
		}
		if (access(formPath.c_str(), F_OK) != 0)
		{
			break ;
		}
		i++;
	}
	if (i == String::npos)
		formPath = tempFormPath + ".json";
	return (formPath);
}

static void	uploadForm(const POSTBody& body, const String& uploadsDir, const String& sid)
{
	String			uploadDest = constructFormPath(uploadsDir, sid);
	stringstream	stream;
	String			str;
	ofstream		outfile;

	outfile.open(uploadDest.c_str());
	if (outfile)
	{
		stream << "{\n";
		for (std::size_t i = 0; i < body.urlEncodedKeys.size(); i++)
		{
			stream << "\t\"" << body.urlEncodedKeys[i] << "\": \"" << body.urlEncodedValues[i] << "\",\n";
		}
		str = stream.str();
		str = str.substr(0, str.length() - 2);
		str += "\n}";
		outfile << str;
	}
	else
	{
		throw InternalServerError500();
	}
}
