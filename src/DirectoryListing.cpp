/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 07:33:32 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/10 08:09:36 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <dirent.h>
#include "Server.hpp"

// TODO: make this look nicer
void	Server::generateDirectoryListing(Response& response, const std::string& dirName) const
{
	DIR*	dir = opendir(dirName.c_str());
	std::cout << dirName << '\n';
	if (dir)
	{
		response.messageBody += "<html>\n";
		response.messageBody += "<head><title>Index of " + dirName + "</title></head>\n";
		response.messageBody += "<body>\n";
		response.messageBody += "<h1>Index of " + dirName + "</h1>\n";
		response.messageBody += "<hr>\n";
		response.messageBody += "<pre>\n";

		dirent*	entry = readdir(dir);
		while (entry != NULL)
		{
			response.messageBody += "<a href=\"";
			response.messageBody += entry->d_name;
			response.messageBody += "\">";

			response.messageBody += entry->d_name;
			response.messageBody += "</a>\n";
			entry = readdir(dir);
		}
		response.messageBody += "</pre>\n";
		response.messageBody += "<hr>\n";
		response.messageBody += "</body>\n";
		response.messageBody += "</html>";
	}
	response.statusCode = 200;
	response.reasonPhrase = "OK";
}

