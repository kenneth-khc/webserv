/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preconditions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:22:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 02:24:15 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <sstream>
#include <ctime>
#include "Time.hpp"
#include "ErrorCode.hpp"
#include "Optional.hpp"
#include "date.hpp"
#include "etag.hpp"
#include "preconditions.hpp"

bool	processPreconditions(const Request &request, const struct stat &statbuf) {
	Optional<String>	value = request["If-None-Match"];

	if (value.exists) {
		String	etag = constructETagHeader(statbuf.st_mtim, statbuf.st_size);

		if (processIfNoneMatchHeader(value.value, etag) == false) {
			if (request.method != Request::GET && request.method != Request::HEAD)
				throw PreconditionFailed412();
			else
				return (false);
		}
		else
			return (true);
	}

	value = request["If-Modified-Since"];

	//	Should check for existence of last modified date
	if (value.exists && (request.method == Request::GET || request.method == Request::HEAD))
		return (processIfModifiedHeader(value.value, statbuf.st_mtim));

	return (true);
}

bool	processIfNoneMatchHeader(const String &value, const String &etag) {
	static const String	optionalWhiteSpaces = " \t";

	//	Have to check for existence of the specified representation for target resource
	if (value.length() == 1 && value[0] == '*')
		return (false);

	std::vector<String>	etagValues = value.split("," + optionalWhiteSpaces);
	std::vector<String>	validETagValues;

	for (std::vector<String>::const_iterator it = etagValues.begin(); it != etagValues.end(); it++) {
		if (isEntityTag(*it) == true)
			validETagValues.push_back(*it);
	}
	if (etagValues.size() == 0)
		return (false);
	for (std::vector<String>::const_iterator it = validETagValues.begin(); it != validETagValues.end(); it++) {
		if (compareETagWeak(etag, *it) == true)
			return (false);
	}
	return (true);
}

bool	processIfModifiedHeader(const String &dateSince, const struct timespec &lastModified) {
	std::tm		timeStruct = {};

	if (isHTTPDate(dateSince, timeStruct) == false)
		return (true);

	std::time_t	dateSinceTime = Time::getTimeSinceEpoch(timeStruct);
	std::time_t	lastModifiedTime = Time::convertToGMT(lastModified.tv_sec);
	if (Time::difftime(lastModifiedTime, dateSinceTime) > 0)
		return (true);
	else
		return (false);
}
