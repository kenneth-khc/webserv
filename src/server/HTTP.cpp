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

#include <iostream>
HTTP::HTTP(const Directive& httpBlock):
	servers(),
	MIMEMappings(httpBlock.getParameterOf("types")
						  .value_or("")),
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
	for (std::map<int,String>::iterator it = errorPages.begin();
		 it != errorPages.end(); ++it)
	{
		std::cout << it->first << " -> " << it->second << '\n';
	}
}

void	HTTP::addServer(const Server& server)
{
	servers.push_back(server);
}
