/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:21:40 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/16 01:28:36 by kecheong         ###   ########.fr       */
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

struct	Location
{
	Location();

	enum { EXACT, PREFIX }	matchType;

	String		uri; // the uri to match

	String		root; // root directory appended to the front of uri

	String		alias; // uri rewritten into alias

	bool		autoindex;

	MediaType	MIMEMappings;

	
};

#endif
