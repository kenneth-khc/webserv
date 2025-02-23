/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 04:33:58 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/24 04:58:19 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <sstream>
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

static String	checkFileName(const String& fileName, const String& sid);
static void		uploadFiles(const POSTBody& body, const String& sid);
static String	checkFormName(const String& sid);
static void		uploadForm(const POSTBody& body, const String& sid);

void	Server::post(Response& response, const Request& request)
{
	if (request.requestTarget == "/")
	{
		vector<String>	values = request.messageBody.split("=");
		const String&	lang = request.cookies.find("lang")->second.value;

		if (lang != values[1])
		{
			response.insert("Set-Cookie", "lang=" + values[1]);
		}
		response.setStatusCode(Response::SEE_OTHER);
		response.insert("Content-Length", 0);
		response.insert("Location", "http://localhost:8000/");
	}
	else if (request.requestTarget == "/pages/form.html")
	{
		POSTBody		msgBody(request);
		const String&	sid = request.cookies.find("sid")->second.value;

		if (msgBody.contentType == "application/x-www-form-urlencoded")
		{
			uploadForm(msgBody, sid);
		}
		else if (msgBody.contentType == "multipart/form-data")
		{
			uploadFiles(msgBody, sid);
		}
		else
		{
			throw UnsupportedMediaType415("Accept", "application/x-www-form-urlencoded, multipart/form-data");
		}
		response.setStatusCode(Response::SEE_OTHER);
		response.insert("Content-Length", 0);
		response.insert("Location", "http://localhost:8000/pages/form.html");
	}
	else
	{
		throw NotFound404();
	}
	// TODO: location header
	// TODO: regenerate the page/link to created resource after POSTing
}

static String	checkFileName(const String& fileName, const String& sid)
{
	Optional<String::size_type>	pos = fileName.find_last_of('.');
	String						name = fileName;
	size_t						i = 0;
	String						extension;
	stringstream				stream;
	String						str;

	if (pos.exists == true)
	{
		extension = "." + fileName.substr(pos.value + 1);
		name = Server::uploadsDir + "/" + sid + "_" + fileName.substr(0, pos.value);
	}
	while (i < String::npos)
	{
		if (i != 0)
		{
			stream << i;
			str = name + stream.str() + extension;
			stream.str("");
		}
		else
		{
			str = name + extension;
		}
		if (access(str.c_str(), F_OK) != 0)
		{
			break ;
		}
		i++;
	}
	return (str);
}

static void	uploadFiles(const POSTBody& body, const String& sid)
{
	String			uploadDest;
	ofstream		outfile;


	for (vector<POSTBodyPart>::const_iterator it = body.parts.begin();
		 it != body.parts.end(); ++it)
	{
		map<String,String>::const_iterator fname = it->contentDisposition.find("filename");
		if (fname != it->contentDisposition.end())
		{
			uploadDest = checkFileName(fname->second, sid);
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

static String	checkFormName(const String& sid)
{
	String			name = Server::uploadsDir + "/" + sid + "_form";
	size_t			i = 0;
	stringstream	stream;
	String			str;

	while (i < String::npos)
	{
		if (i != 0)
		{
			stream << i;
			str = name + stream.str() + ".json";
			stream.str("");
		}
		else
		{
			str = name + ".json";
		}
		if (access(str.c_str(), F_OK) != 0)
		{
			break ;
		}
		i++;
	}
	return (str);
}

static void	uploadForm(const POSTBody& body, const String& sid)
{
	String			uploadDest = checkFormName(sid);
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
