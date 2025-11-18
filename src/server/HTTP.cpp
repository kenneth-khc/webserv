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
#include "VectorInitializer.hpp"

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

HTTP::HTTP(const Directive& httpBlock):
	servers(),

	// TODO(kecheong): default mime types file?
	// TODO(kecheong): mime.types should be relative to where the config file is
	// and not where the process is ran on?
	// or should it be appended to prefix?
	MIMEMappings(httpBlock.getParameterOf("types")
						  .value_or("mime.types")),

	autoindex(httpBlock.getParameterOf("autoindex")
					   .transform(String::toBool)
					   .value_or(false)),

	rootDirectory(httpBlock.getParameterOf("root")
						   .value_or("html")),

	indexFiles(httpBlock.getParametersOf("index")
						.value_or(vector_of<String>("index.html"))),

	clientMaxBodySize(httpBlock.getParameterOf("client_max_body_size")
							   .transform(String::toSize)
							   .value_or(1000000))
{
	errorPages = httpBlock.generateErrorPagesMapping()
						  .value_or(std::map<int, String>());
}

void	HTTP::addServer(const Server& server)
{
	servers.push_back(server);
}
