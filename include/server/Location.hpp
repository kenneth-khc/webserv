/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:21:40 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/28 19:03:55 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

/* A Location block, defining how Requests for specific URI paths should be
 * handled and processed. */

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

	/** whether we should accept uploads */
	bool				acceptUploads;

	/** where uploads for our web server is stored */
	String				uploadDirectory;

	RedirectHandler	redirectHandler;

	void	checkIfAllowedMethod(const String&) const;
};

#endif
