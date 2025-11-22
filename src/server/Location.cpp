/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:41:03 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/23 00:59:08 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Defaults.hpp"
#include "ErrorCode.hpp"
#include "RedirectHandler.hpp"
#include <algorithm>

Location::Location():
uri("/"),
root(Defaults::ROOT),
indexFiles(Defaults::INDEX),
autoindex(Defaults::AUTOINDEX.toBool()),
allowedMethods(Defaults::ALLOW_METHODS),
MIMEMappings(Defaults::TYPES),
clientMaxBodySize(Defaults::CLIENT_MAX_BODY_SIZE.toSize()),
uploadDirectory(Defaults::UPLOAD_DIRECTORY),
acceptUploads(!uploadDirectory.empty()),
redirectHandler()
{
}

Location::Location(const Directive& block):
uri(block.getParameter()),
root(block.getInherited("root", Defaults::ROOT)),
indexFiles(block.getInherited("index", Defaults::INDEX)),
autoindex(block.getInherited("autoindex", Defaults::AUTOINDEX).toBool()),
allowedMethods(block.getInherited("allow_method", Defaults::ALLOW_METHODS)),
MIMEMappings(block.getInherited("types", Defaults::TYPES)),
clientMaxBodySize(block.getInherited("client_max_body_size", Defaults::CLIENT_MAX_BODY_SIZE).toSize()),
uploadDirectory(block.getInherited("upload_directory", Defaults::UPLOAD_DIRECTORY)),
acceptUploads(!uploadDirectory.empty()),
redirectHandler(block)
{
	errorPages = block.generateErrorPagesMapping()
							  .value_or(std::map<int,String>());
}

bool	Location::shouldRedirect() const
{
	return redirectHandler.shouldRedirect;
}

void	Location::executeRedirection(Response& response) const
{
	return redirectHandler.executeRedirection(response);
}
