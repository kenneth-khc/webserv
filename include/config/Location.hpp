/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:21:40 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/20 02:24:40 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

/* A Location block, defining how Requests for specific URI paths should be
 * handled and processed. */

/* Precedence: exact matches > prefix matches > regex matches */
/* Nested locations inherit from parent unless overriden */

/* Directives to focus on:
 * - root, alias, index, autoindex, error_pages for static files
 * - access control like allow, deny, limit_except
 * - redirects with return or rewrite
 * - client max body size
 * - cgi
 */

#include "String.hpp"
#include "MediaType.hpp"
#include "Directive.hpp"

struct	Location
{
	Location();
	Location(const Directive&);

	String					uri; // the uri to match

	String					root; // root directory appended to the front of uri

	bool					autoindex;

	MediaType				MIMEMappings;

	std::vector<String>		indexFiles;

	std::vector<String>		allowedMethods;

	std::size_t				clientMaxBodySize;

	std::map<int,String>	errorPages;

	bool					acceptUploads;

	String					uploadDirectory;
};

#endif
