/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:21:40 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/23 00:48:15 by cteoh            ###   ########.fr       */
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
#include "RedirectHandler.hpp"

struct	Location
{
	Location();
	Location(const Directive&);

	/** URI to match with the incoming request to determine whether this block
		should handle the request */
	String				uri;

	/** root directory of the local filesystem */
	String				root;

	String				alias;

	/** a list of index files to try when a directory is requested */
	std::vector<String>	indexFiles;

	/** whether to generate directory listings when no index files are found */
	bool				autoindex;

	/** pages to serve when responding with an error code */
	std::map<int,String>errorPages;

	/** the allowed HTTP request methods */
	std::vector<String>	allowedMethods;

	MediaType			MIMEMappings;

	/** max size allowed for the incoming request body */
	std::size_t			clientMaxBodySize;

	/** where uploads for our web server is stored */
	String				uploadDirectory;

	/** whether we should accept uploads */
	bool				acceptUploads;

	/** forwards to redirectHandler to check if should redirect */
	bool	shouldRedirect() const;
	/** forwards to redirectHandler to perform redirection */
	void	executeRedirection(Response&) const;

private:
	RedirectHandler	redirectHandler;
};

#endif
