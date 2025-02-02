/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 04:33:58 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/02 05:56:39 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include "Server.hpp"
#include "debugUtils.hpp"
#include "MessageBody.hpp"
#include <map>
#include <string>

struct	ContentType
{
	std::string	type;
	std::string	boundary;
	std::string	boundaryEnd;
};

std::string	getContentTypeLine(const Request& request)
{
	std::map<std::string,std::string>::const_iterator it;

	it = request.headers.find("Content-Type");
	if (it != request.headers.end())
	{
		return it->second;
	}
	else
	{
		return "";
	}
}

ContentType	getContentType(const Request& request)
{
	ContentType	contentType;
	std::string	contentTypeLine = getContentTypeLine(request);
	if (contentTypeLine != "")
	{
		size_t	i = 0;
		while (contentTypeLine[i] != ';')
		{
			contentType.type += contentTypeLine[i];
			++i;
		}
		i += 2;
		while (contentTypeLine[i] != '-')
			++i;
		while (i != contentTypeLine.size())
		{
			contentType.boundary += contentTypeLine[i];
			++i;
		}
		contentType.boundaryEnd = contentType.boundary + "--";
	}
	return contentType;
}

// TODO: make this work with all types of files instead of just text/plain
bool	uploadFile(const Request& request, const ContentType& contentType, std::string filename)
{
	const std::string&	body = request.messageBody;
	size_t	pos = body.find("\r\n\r\n") + 4;
	size_t	end = body.find("\r\n--" + contentType.boundaryEnd);
	size_t	contentSize = end - pos;
	std::vector<char>	contents;
	contents.resize(contentSize);
	// TODO: route to proper upload destinations ??
	std::string	uploadDest = "uploads/";
	uploadDest += filename;
	std::cout << "Uploading to " << uploadDest << '\n';
	std::ofstream	outfile;
	outfile.open(uploadDest.c_str());
	if (outfile)
	{
		for (; pos < end; ++pos)
		{
			outfile << request.messageBody[pos];
		}
		std::cout << "Uploaded " << uploadDest << '\n';
		return true;
	}
	else
	{
		std::cerr << "Upload failed.\nConsider checking the upload destination.\n";
		return false;
	}
}

std::string	getFilenameFromContentDisposition(const std::string& messageBody)
{
	size_t begin = messageBody.find("Content-Disposition");
	if (begin == messageBody.npos)
		return "";
	std::string	contentDispositionLine = messageBody.substr(begin);
	contentDispositionLine = contentDispositionLine.substr(0,
			contentDispositionLine.find("\r\n"));

	size_t	filenameBegin = contentDispositionLine.find("filename=");
	std::string	filenameKeyValue = contentDispositionLine.substr(filenameBegin);
	size_t eqs = filenameKeyValue.find('=');
	std::string	filename = filenameKeyValue.substr(eqs+1);
	if (filename[0] == '"' && filename[filename.size()-1] == '"')
	{
		filename = filename.substr(1, filename.size()-2);
	}
	return filename;
}

void	Server::post(Response& response, const Request& request) const
{
	/*MessageBody	msgBody = request.messageBody;*/
	ContentType	contentType = getContentType(request);
	if (contentType.type == "multipart/form-data")
	{
		std::string	filename = getFilenameFromContentDisposition(request.messageBody);
		if (uploadFile(request, contentType, filename))
		{
			response.statusCode = 200;
			response.reasonPhrase = "OK";
			// TODO: generate location dynamically
			std::string	location = "http://localhost:8000/upload/" + filename;
			response.headers.insert(std::make_pair("Location", location));
			// TODO: regenerate the page/link to created resource after POSTing
			response.messageBody = getFileContents("html/form.html");
		}
	}
	return ;
}









