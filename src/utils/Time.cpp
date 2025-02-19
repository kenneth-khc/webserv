/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:34:45 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/19 07:30:03 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Time.hpp"

Time::Time(void) {}

Time::~Time(void) {}

Time::Time(const Time &obj) {
	(void)obj;
}

Time	&Time::operator=(const Time &obj) {
	(void)obj;
	return (*this);
}

std::time_t Time::getTimeSinceEpoch(void) {
	return (std::time(NULL));
}

std::time_t	Time::getTimeSinceEpoch(std::tm &time) {
	return (std::mktime(&time));
}

std::time_t	Time::convertToGMT(const std::time_t &time) {
	return (std::mktime(std::gmtime(&time)));
}

String	Time::printHTTPDate(void) {
	char	buffer[sizeof("DAY, DD MMM YYYY HH:MM:SS GMT")];

	std::time_t	time = std::time(NULL);
	std::tm		*timeStruct = std::gmtime(&time);

	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeStruct);
	return (buffer);
}

String	Time::printHTTPDate(const struct timespec &time) {
	char	buffer[sizeof("DAY, DD MMM YYYY HH:MM:SS GMT")];

	std::tm	*timeStruct = std::gmtime(&time.tv_sec);

	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeStruct);
	return (buffer);
}

double	Time::difftime(const std::time_t timeOne, const std::time_t timeTwo) {
	return (timeOne - timeTwo);
}

String	Time::printAutoindexDate(const struct timespec &time) {
	char	buffer[sizeof("DD-MMM-YYYY HH:MM")];

	std::tm	*timeStruct = std::gmtime(&time.tv_sec);

	std::strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", timeStruct);
	return (buffer);
}
