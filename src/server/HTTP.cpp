/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTP.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 07:22:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 08:15:41 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP.hpp"
#include "Defaults.hpp"

HTTP::HTTP()
{
}

HTTP&	HTTP::operator=(const HTTP& rhs)
{
	if (this != &rhs)
	{
		servers = rhs.servers;
		MIMEMappings = rhs.MIMEMappings;
		autoindex = rhs.autoindex;
		rootDirectory = rhs.rootDirectory;
		indexFiles = rhs.indexFiles;
		clientMaxBodySize = rhs.clientMaxBodySize;
		errorPages = rhs.errorPages;
	}
	return *this;
}

HTTP::HTTP(const Directive& block):
servers(),
MIMEMappings(block.getInherited("types", Defaults::TYPES)),
autoindex(block.getInherited("autoindex", Defaults::AUTOINDEX).toBool()),
rootDirectory(block.getInherited("root", Defaults::ROOT)),
indexFiles(block.getInherited("index", Defaults::INDEX)),
clientMaxBodySize(block.getInherited("client_max_body_size", Defaults::CLIENT_MAX_BODY_SIZE).toSize())
{
	errorPages = block.generateErrorPagesMapping()
						  .value_or(std::map<int, String>());
}

void	HTTP::addServer(const Server& server)
{
	servers.push_back(server);
}
