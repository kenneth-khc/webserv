/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preconditions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:22:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/06 21:50:42 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include "Time.hpp"
#include "Optional.hpp"
#include "date.hpp"
#include "etag.hpp"
#include "preconditions.hpp"

bool	processPreconditions(const Request &request, const struct stat &statbuf) {
	std::string	cacheControl = request["Cache-Control"].value;

	if (cacheControl.find("no-cache") != std::string::npos)
		return (true);

	Optional<std::string>	value = request["If-None-Match"];

	if (value.exists) {
		std::string	etag = constructETagHeader(statbuf.st_mtim, request.find< Optional<int> >("Content-Length"));

		if (processIfNoneMatchHeader(value.value, etag))
	}
	else {
		value = request["If-Modified-Since"];

		if (value.exists) {
			processIfModifiedHeader(value.value, statbuf.st_mtim);

			
		}
	}

}

bool	processIfNoneMatchHeader(const std::string &value, const std::string &etag) {
	if (value.length() == 0)
		return (true);
	if (value == etag)
		return (false);
	else
		return (true);
}

bool	processIfModifiedHeader(const std::string &dateSince, const struct timespec &lastModified) {
	std::tm		timeStruct = {};

	if (isDate(dateSince, timeStruct) == false)
		return (true);

	std::time_t	dateSinceTime = Time::getTimeSinceEpoch(timeStruct);
	std::time_t	lastModifiedTime = Time::convertToGMT(lastModified.tv_sec);
	if (Time::difftime(lastModifiedTime, dateSinceTime) > 0)
		return (true);
	else
		return (false);
}
