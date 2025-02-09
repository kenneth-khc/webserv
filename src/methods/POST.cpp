/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 04:33:58 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/10 01:12:54 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <string>
#include "Server.hpp"
#include "MessageBody.hpp"

using std::string;
using std::ofstream;

static void	uploadFiles(const MessageBody& body);
static bool	uploadFile(const std::string& filename, const String& fileContent);

void	Server::post(Response& response, const Request& request) const
{
	MessageBody	msgBody(request);

	if (msgBody.contentType == "multipart/form-data")
	{
		uploadFiles(msgBody);
	}
	response.statusCode = 200;
	response.reasonPhrase = "OK";
	response.messageBody = getFileContents("html/form.html");
}

static void	uploadFiles(const MessageBody& body)
{
	for (vector<MessageBodyPart>::const_iterator it = body.parts.begin();
		 it != body.parts.end(); ++it)
	{
		map<String,String>::const_iterator fname = it->contentDisposition.find("filename");
		if (fname != it->contentDisposition.end())
		{
			uploadFile(fname->second, it->content);
		}
	}
}

// TODO: route to proper upload destinations ??
static bool	uploadFile(const std::string& filename, const String& fileContent)
{
	string	uploadDest = "uploads/";
	uploadDest += filename;
	std::cout << "Uploading to " << uploadDest << '\n';
	ofstream	outfile;
	outfile.open(uploadDest.c_str());
	if (outfile)
	{
		outfile << fileContent;
		std::cout << "Uploaded " << uploadDest << '\n';
		return true;
	}
	else
	{
		std::cerr << "Upload failed.\nConsider checking the upload destination.\n";
		return false;
	}
}
