/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ifModified.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 01:27:15 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 05:19:23 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <ctime>
#include <cstring>
#include "date.hpp"
#include "ifModified.hpp"

bool	processIfModifiedHeader(const std::string &dateSince, const struct timespec &lastModified) {
	std::tm	time;

	std::memset(&time, 0, sizeof(std::tm));
	if (isDate(dateSince, time) == false)
		return (false);

	std::time_t	dateSinceInEpoch = std::mktime(&time);
	std::time_t	lastModifiedInUTC = std::mktime(std::gmtime(&lastModified.tv_sec));
	if (std::difftime(lastModifiedInUTC, dateSinceInEpoch) > 0)
		return (true);
	else
		return (false);
}

std::string	convertLastModifiedToHTTPDate(const struct timespec &lastModified) {
	std::tm	*lastModifiedInUTC = std::gmtime(&lastModified.tv_sec);
	char	buffer[sizeof("DAY, DD MM YYYY HH:MM:SS") + 1];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S", lastModifiedInUTC);

	return (std::string(buffer) + " GMT");
}
