/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:41:03 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:00:48 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "VectorInitializer.hpp"
#include "ErrorCode.hpp"
#include <algorithm>

Location::Location():
	matchType(PREFIX),
	root("html"),
	alias(),
	autoindex(false),
	MIMEMappings("mime.types"),
	indexFiles()
{

}

Location::Location(const Directive& locationBlock):
	matchType(PREFIX),
	// TODO: location exact matches
	uri(locationBlock.parameters[0].value),

	root(locationBlock.recursivelyLookup<String>("root")
					  .value_or("html")),

	alias(),

	autoindex(locationBlock.recursivelyLookup<String>("autoindex")
						   .transform(String::toBool)
						   .value_or(false)),

	MIMEMappings(locationBlock.recursivelyLookup<String>("types")
							  .value_or("")),

	indexFiles(locationBlock.recursivelyLookup< std::vector<String> >("index")
							.value_or(vector_of<String>("index.html"))),

	allowedMethods(locationBlock.getParametersOf("allow_method")
								.value_or(vector_of<String>("GET"))),

	clientMaxBodySize(locationBlock.recursivelyLookup<String>("client_max_body_size")
								   .transform(String::toSize)
								   .value_or(1000000)),

	executeCGI(locationBlock.getParameterOf("exec_CGI")
							.transform(String::toBool)
							.value_or(false)),

	CGIScriptFileExtensions(locationBlock.getParametersOf("CGI_script")
										 .value_or(vector_of<String>())),

	acceptUploads(locationBlock.getParameterOf("accept_uploads")
							   .transform(String::toBool)
							   .value_or(false)),

	uploadDirectory(locationBlock.getParameterOf("upload_directory")
								 .value_or(""))
{
	errorPages = locationBlock.generateErrorPagesMapping()
							  .value_or(std::map<int,String>());
}

void	Location::checkIfAllowedMethod(const String& method) const
{
	if (std::find(allowedMethods.begin(),
				  allowedMethods.end(),
				  method) == allowedMethods.end())
	{
		throw MethodNotAllowed405();
	}
}
