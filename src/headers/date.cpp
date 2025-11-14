/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 02:14:54 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:07:55 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <cstdlib>
#include "date.hpp"

bool	isDateHeader(const String &date) {
	std::tm	time = {};

	if (isHTTPDate(time, date) == true)
		return (true);
	return (false);
}

bool	isHTTPDate(std::tm &time, const String &date) {
	if (date.length() == 0)
		return (false);
	if (isIMFFixDate(time, date) == true)
		return (true);
	if (isObsoleteDate(time, date) == true)
		return (true);
	return (false);
}

bool	isIMFFixDate(std::tm &time, const String &date) {
	std::stringstream	stream(date);
	String				str;
	String				buffer;

	if (!String::getline(stream, str, ','))
		return (false);
	if (isDayName(time, str) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (!String::getline(stream, str, ' '))
		return (false);
	buffer = str;
	if (!String::getline(stream, str, ' '))
		return (false);
	buffer += " " + str;
	if (!String::getline(stream, str, ' '))
		return (false);
	buffer += " " + str;
	if (isDateOne(time, buffer) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isTimeOfDay(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str != "GMT")
		return (false);
	return (true);
}

bool	isDayName(std::tm &time, const String &day) {
	const String	days[NUM_OF_DAYS] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};

	for (int i = 0; i < NUM_OF_DAYS; i++) {
		if (day == days[i]) {
			time.tm_wday = i;
			return (true);
		}
	}
	return (false);
}

bool	isDateOne(std::tm &time, const String &date) {
	if (date.length() == 0)
		return (false);

	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isDay(time, str) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isMonth(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (isYear(time, str) == false)
		return (false);
	return (true);
}

bool	isDay(std::tm &time, const String &day) {
	if (day.length() != 2)
		return (false);

	for (String::size_type i = 0; i < day.length(); i++) {
		if (std::isdigit(day[i]) == 0)
			return (false);
	}
	time.tm_mday = std::atoi(day.c_str());
	return (true);
}

bool	isMonth(std::tm &time, const String &month) {
	const String	months[NUM_OF_MONTHS] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	for (int i = 0; i < NUM_OF_MONTHS; i++) {
		if (month == months[i]) {
			time.tm_mon = i;
			return (true);
		}
	}
	return (false);
}

bool	isYear(std::tm &time, const String &year) {
	if (year.length() != 4)
		return (false);

	for (String::size_type i = 0; i < year.length(); i++) {
		if (std::isdigit(year[i]) == 0)
			return (false);
	}
	time.tm_year = std::atoi(year.c_str()) - 1900;
	return (true);
}

bool	isTimeOfDay(std::tm &time, const String &timeOfDay) {
	std::stringstream	stream(timeOfDay);
	String				str;

	if (!String::getline(stream, str, ':'))
		return (false);
	if (isHour(time, str) == false)
		return (false);

	if (!String::getline(stream, str, ':'))
		return (false);
	if (isMinute(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (isSecond(time, str) == false)
		return (false);
	return (true);
}

bool	isHour(std::tm &time, const String &hour) {
	if (hour.length() != 2)
		return (false);

	for (String::size_type i = 0; i < hour.length(); i++) {
		if (std::isdigit(hour[i]) == 0)
			return (false);
	}
	time.tm_hour = std::atoi(hour.c_str());
	return (true);
}

bool	isMinute(std::tm &time, const String &minute) {
	if (minute.length() != 2)
		return (false);

	for (String::size_type i = 0; i < minute.length(); i++) {
		if (std::isdigit(minute[i]) == 0)
			return (false);
	}
	time.tm_min = std::atoi(minute.c_str());
	return (true);
}

bool	isSecond(std::tm &time, const String &second) {
	if (second.length() != 2)
		return (false);

	for (String::size_type i = 0; i < second.length(); i++) {
		if (std::isdigit(second[i]) == 0)
			return (false);
	}
	time.tm_sec = std::atoi(second.c_str());
	return (true);
}

bool	isObsoleteDate(std::tm &time, const String &date) {
	if (isRFC850Date(time, date) == false)
		return (false);
	if (isASCTimeDate(time, date) == false)
		return (false);
	return (true);
}

bool	isRFC850Date(std::tm &time, const String &date) {
	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, ','))
		return (false);
	if (isDayNameOne(time, str) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (!String::getline(stream, str, ' '))
		return (false);
	if (isDateTwo(time, str) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isTimeOfDay(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str != "GMT")
		return (false);
	return (true);
}

bool	isDayNameOne(std::tm &time, const String &day) {
	const String	days[NUM_OF_DAYS] = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
		"Friday", "Saturday"
	};

	for (int i = 0; i < NUM_OF_DAYS; i++) {
		if (day == days[i]) {
			time.tm_wday = i;
			return (true);
		}
	}
	return (false);
}

bool	isDateTwo(std::tm &time, const String &date) {
	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, '-'))
		return (false);
	if (isDay(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '-'))
		return (false);
	if (isMonth(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str.length() != 2)
		return (false);
	for (String::size_type i = 0; i < str.length(); i++) {
		if (std::isdigit(str[i]) == 0)
			return (false);
	}

	std::time_t	currTime = std::time(0);
	std::tm		*currTimeStruct = std::gmtime(&currTime);

	time.tm_year = (currTimeStruct->tm_year / 100) * 100 + std::atoi(str.c_str());
	return (true);
}

bool	isASCTimeDate(std::tm &time, const String &date) {
	std::stringstream	stream(date);
	String				str;
	String				buffer;

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isDayName(time, str) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	buffer = str;
	if (!String::getline(stream, str, ' '))
		return (false);
	if (str.length() == 0) {
		if (!String::getline(stream, str, ' '))
			return (false);
		buffer += ' ';
	}
	buffer += " " + str;
	if (isDateThree(time, buffer) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isTimeOfDay(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (isYear(time, str) == false)
		return (false);
	return (true);
}

bool	isDateThree(std::tm &time, const String &date) {
	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isMonth(time, str) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str[0] == ' ')
		str = str.substr(1);
	for (String::size_type i = 0; i < str.length(); i++) {
		if (std::isdigit(str[i]) == 0)
			return (false);
	}
	time.tm_mday = std::atoi(str.c_str());
	return (true);
}
