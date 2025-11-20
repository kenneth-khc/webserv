/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:41:03 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/20 02:31:05 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "VectorInitializer.hpp"
#include "ErrorCode.hpp"
#include <algorithm>

Location::Location():
	root("html"),
	indexFiles(),
	autoindex(false),
	MIMEMappings("mime.types")
{
}

Location::Location(const Directive& locationBlock):
	uri(locationBlock.parameters[0].value),

	root(locationBlock.recursivelyLookup<String>("root")
					  .value_or("html")),

	indexFiles(locationBlock.recursivelyLookup< std::vector<String> >("index")
							.value_or(vector_of<String>("index.html"))),

	autoindex(locationBlock.recursivelyLookup<String>("autoindex")
						   .transform(String::toBool)
						   .value_or(false)),

	allowedMethods(locationBlock.getParametersOf("allow_method")
								.value_or(vector_of<String>("GET"))),

	MIMEMappings(locationBlock.recursivelyLookup<String>("types")
							  .value_or("mime.types")),

	clientMaxBodySize(locationBlock.recursivelyLookup<String>("client_max_body_size")
								   .transform(String::toSize)
								   .value_or(1000000)),

	// TODO(kecheong): accept_uploads is outdated. check for existence of
	// "upload_directory" instead
	acceptUploads(locationBlock.getParameterOf("accept_uploads")
							   .transform(String::toBool)
							   .value_or(false)),

	uploadDirectory(locationBlock.getParameterOf("upload_directory")
								 .value_or(""))
{
	errorPages = locationBlock.generateErrorPagesMapping()
							  .value_or(std::map<int,String>());
}
