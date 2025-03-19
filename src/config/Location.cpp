/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:41:03 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/20 06:36:25 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location():
	matchType(PREFIX),
	root("html"),
	alias(),
	autoindex(false),
	MIMEMappings("mime.types"),
	indexFiles()
{

}

bool	toBool(const String& str)
{
	if (str == "on")
	{
		return true;
	}
	else
	{
		return false;
	}
}

Location::Location(const Directive& directive):
	matchType(PREFIX),
	// TODO: location exact matches
	uri(directive.parameters[0]),
	root(directive.recursivelyLookup("root")
				  .value_or("html")),
	alias(),
	autoindex(directive.recursivelyLookup("autoindex")
					   .transform(toBool)
					   .value_or(false)),
	MIMEMappings("mime.types")
{

}

