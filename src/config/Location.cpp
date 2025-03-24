/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:41:03 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 07:30:17 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "VectorInitializer.hpp"

Location::Location():
	matchType(PREFIX),
	root("html"),
	alias(),
	autoindex(false),
	MIMEMappings("mime.types"),
	indexFiles()
{

}

Location::Location(const Directive& directive):
	matchType(PREFIX),
	// TODO: location exact matches
	uri(directive.parameters[0]),

	root(directive.recursivelyLookup<String>("root")
				  .value_or("html")),

	alias(),

	autoindex(directive.recursivelyLookup<String>("autoindex")
					   .transform(String::toBool)
					   .value_or(false)),

	MIMEMappings("mime.types"),

	indexFiles(directive.recursivelyLookup< std::vector<String> >("index")
						.value_or(vector_of<String>("index.html"))),

	allowedMethods(directive.getParametersOf("allow_method")
							.value_or(vector_of<String>("GET"))),

	clientMaxBodySize(directive.recursivelyLookup<String>("client_max_body_size")
								 .transform(String::toSize)
								 .value_or(1000000))
{
}

