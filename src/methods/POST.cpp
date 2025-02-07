/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 04:33:58 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/08 05:35:40 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <string>
#include "Server.hpp"
#include "MessageBody.hpp"

// TODO: route to proper upload destinations ??
bool	uploadFile(std::string filename, String& fileContent)
{
	std::string	uploadDest = "uploads/";
	uploadDest += filename;
	std::cout << "Uploading to " << uploadDest << '\n';
	std::ofstream	outfile;
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

void	Server::post(Response& response, const Request& request) const
{
	MessageBody	msgBody(request);

	for (std::vector<MessageBodyChunk>::iterator it = msgBody.chunks.begin();
		 it != msgBody.chunks.end(); ++it)
	{
		std::map<String,String>::iterator fname = it->contentDisposition.find("filename");
		if (fname != it->contentDisposition.end())
		{
			uploadFile(fname->second, it->content);
		}
	}
	response.statusCode = 400;
	response.reasonPhrase = "OK";
	response.messageBody = getFileContents("html/form.html");
}

