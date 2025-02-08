/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preconditions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:22:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/08 04:05:34 by cteoh            ###   ########.fr       */
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
	Optional<std::string>	value = request["If-None-Match"];

	if (value.exists) {
		std::string	etag = constructETagHeader(statbuf.st_mtim, statbuf.st_size);

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

bool	processIfNoneMatchHeader(const std::string &value, const std::string &etag) {
	static const std::string	optionalWhiteSpaces = " \t";

	//	Have to check for existence of the specified representation for target resource
	if (value.length() == 1 && value[0] == '*')
		return (false);

	std::stringstream			stream(value);
	std::string					str;
	std::size_t					frontPos;
	std::size_t					backPos;
	std::vector<std::string>	values;

	while (std::getline(stream, str, ',')) {
		frontPos = str.find_first_not_of(optionalWhiteSpaces);
		if (frontPos == std::string::npos)
			continue ;
		backPos = str.find_last_not_of(optionalWhiteSpaces);
		str = str.substr(frontPos, backPos + 1 - frontPos);
		if (isEntityTag(str) == true)
			values.push_back(str);
	}
	if (values.size() == 0)
		return (false);
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); it++) {
		if (compareETagWeak(etag, *it) == true)
			return (false);
	}
	return (true);
}

bool	processIfModifiedHeader(const std::string &dateSince, const struct timespec &lastModified) {
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
