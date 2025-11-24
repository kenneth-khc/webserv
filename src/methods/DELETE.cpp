/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:14:43 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/21 05:14:32 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include "String.hpp"
#include "ErrorCode.hpp"
#include "Server.hpp"
#include "Driver.hpp"

// TODO: consider other status codes. 200 OK? 202 Accepted? 403 Forbidden?
/*
	Checks for file or directory existence and removes it.
*/
void	Server::delete_(Response& response, const Request& request) const
{
	struct stat	filesystem;

	if (!stat(request.location->uri.c_str(), &filesystem))
	{
		throw NotFound404();
	}

	if (!std::remove(request.location->uri.c_str()))
	{
		response.setStatusCode(Response::NO_CONTENT);
		response.insert("Content-Length", 0);
	} else
	{
		throw InternalServerError500();
	}
}
