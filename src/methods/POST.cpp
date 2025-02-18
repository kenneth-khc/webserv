/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 04:33:58 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/19 07:36:18 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <sstream>
#include <string>
#include "Optional.hpp"
#include "Time.hpp"
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "POSTBody.hpp"

using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::time_t;

static bool		uploadFiles(Response &response, const POSTBody& body);
static bool		uploadFile(const std::string& filename, const String& fileContent);
static bool		uploadForm(Response &response, const POSTBody& body);
static String	getUploadStatusPage(const bool &uploadStatus);

void	Server::post(Response& response, const Request& request)
{
	Session	&session = *(response.currSession);

	if (request.requestTarget == "/")
	{
		vector<String>	values = request.messageBody.split("=");
		String			&lang = session.find("lang")->value;

		if (lang != values[1])
		{
			lang = values[1];
			response.insert("Set-Cookie", "lang=" + values[1]);
		}
		response.setStatusCode(Response::SEE_OTHER);
		response.insert("Content-Length", 0);
		response.insert("Location", "http://localhost:8000/");
	}
	else if (request.requestTarget == "/pages/form.html")
	{
		POSTBody	msgBody(request);

		if (msgBody.contentType == "application/x-www-form-urlencoded")
		{
			response.messageBody = getUploadStatusPage(uploadForm(response, msgBody));
		}
		else if (msgBody.contentType == "multipart/form-data")
		{
			response.messageBody = getUploadStatusPage(uploadFiles(response, msgBody));
		}
		else
			throw UnsupportedMediaType415("Accept", "application/x-www-form-urlencoded, multipart/form-data");

		response.setStatusCode(Response::CREATED);
		response.insert("Content-Length", response.messageBody.length());
		response.insert("Location", "http://localhost:8000/pages/form.html");
	}
	// TODO: location header
	// TODO: regenerate the page/link to created resource after POSTing
}

static bool	uploadFiles(Response& response, const POSTBody& body)
{
	String			name;
	stringstream	stream;
	bool			uploadStatus = true;

	for (vector<POSTBodyPart>::const_iterator it = body.parts.begin();
		 it != body.parts.end(); ++it)
	{
		map<String,String>::const_iterator fname = it->contentDisposition.find("filename");
		if (fname != it->contentDisposition.end())
		{
			name = (*response.currSession)["sid"].value + "_";
			stream << Time::getTimeSinceEpoch();
			name += "_" + stream.str() + "_";
			if (uploadFile(name + fname->second, it->content) == false)
			{
				uploadStatus = false;
			}
			stream.str("");
		}
	}
	return (uploadStatus);
}

// TODO: route to proper upload destinations ??
static bool	uploadFile(const std::string& filename, const String& fileContent)
{
	string		uploadDest = Server::uploadsDir + "/" + filename;
	ofstream	outfile;

	outfile.open(uploadDest.c_str());
	if (outfile)
	{
		outfile << fileContent;
		return true;
	}
	else
	{
		return false;
	}
}

static bool	uploadForm(Response &response, const POSTBody& body)
{
	static std::size_t	i = 0;
	stringstream		stream;
	string				str;
	string				uploadDest = Server::uploadsDir + "/" + (*response.currSession)["sid"].value + "_";
	ofstream			outfile;

	if (i != 0)
	{
		stream << i;
		stream >> str;
		stream.clear();
	}
	uploadDest += "form" + str + ".json";
	outfile.open(uploadDest.c_str());
	if (outfile)
	{
		stream << "{\n";
		for (std::size_t i = 0; i < body.urlEncodedKeys.size(); i++)
		{
			stream << "\t\"" << body.urlEncodedKeys[i] << "\": \"" << body.urlEncodedValues[i] << "\",\n";
		}
		std::getline(stream, str, '\0');
		str = str.substr(0, str.length() - 2);
		str += "\n}";
		outfile << str;
		i++;
		return true;
	}
	else
	{
		return false;
	}
}

static String	getUploadStatusPage(const bool &uploadStatus)
{
	ifstream	infile;
	String		pageContent;

	if (uploadStatus == true)
	{
		infile.open("misc_pages/upload_success.html");
	}
	else
	{
		infile.open("misc_pages/upload_failed.html");
	}
	String::getline(infile, pageContent, '\0');
	return pageContent;
}
