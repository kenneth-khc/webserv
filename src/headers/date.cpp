/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 02:14:54 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 02:19:18 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <cstdlib>
#include "date.hpp"

bool	isDateHeader(const String &date) {
	std::tm	time = {};

	if (isHTTPDate(date, time) == true)
		return (true);
	return (false);
}

bool	isHTTPDate(const String &date, std::tm &time) {
	if (date.length() == 0)
		return (false);
	if (isIMFFixDate(date, time) == true)
		return (true);
	if (isObsoleteDate(date, time) == true)
		return (true);
	return (false);
}

bool	isIMFFixDate(const String &date, std::tm &time) {
	std::stringstream	stream(date);
	String				str;
	String				buffer;

	if (!String::getline(stream, str, ','))
		return (false);
	if (isDayName(str, time) == false)
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
	if (isDateOne(buffer, time) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isTimeOfDay(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str != "GMT")
		return (false);
	return (true);
}

bool	isDayName(const String &day, std::tm &time) {
	static const String	days[NUM_OF_DAYS] = {
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

bool	isDateOne(const String &date, std::tm &time) {
	if (date.length() == 0)
		return (false);

	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isDay(str, time) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isMonth(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (isYear(str, time) == false)
		return (false);
	return (true);
}

bool	isDay(const String &day, std::tm &time) {
	if (day.length() != 2)
		return (false);

	for (std::size_t i = 0; i < day.length(); i++) {
		if (std::isdigit(day[i]) == 0)
			return (false);
	}
	time.tm_mday = std::atoi(day.c_str());
	return (true);
}

bool	isMonth(const String &month, std::tm &time) {
	static const String	months[NUM_OF_MONTHS] = {
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

bool	isYear(const String &year, std::tm &time) {
	if (year.length() != 4)
		return (false);

	for (std::size_t i = 0; i < year.length(); i++) {
		if (std::isdigit(year[i]) == 0)
			return (false);
	}
	time.tm_year = std::atoi(year.c_str()) - 1900;
	return (true);
}

bool	isTimeOfDay(const String &timeOfDay, std::tm &time) {
	std::stringstream	stream(timeOfDay);
	String				str;

	if (!String::getline(stream, str, ':'))
		return (false);
	if (isHour(str, time) == false)
		return (false);

	if (!String::getline(stream, str, ':'))
		return (false);
	if (isMinute(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (isSecond(str, time) == false)
		return (false);
	return (true);
}

bool	isHour(const String &hour, std::tm &time) {
	if (hour.length() != 2)
		return (false);

	for (std::size_t i = 0; i < hour.length(); i++) {
		if (std::isdigit(hour[i]) == 0)
			return (false);
	}
	time.tm_hour = std::atoi(hour.c_str());
	return (true);
}

bool	isMinute(const String &minute, std::tm &time) {
	if (minute.length() != 2)
		return (false);

	for (std::size_t i = 0; i < minute.length(); i++) {
		if (std::isdigit(minute[i]) == 0)
			return (false);
	}
	time.tm_min = std::atoi(minute.c_str());
	return (true);
}

bool	isSecond(const String &second, std::tm &time) {
	if (second.length() != 2)
		return (false);

	for (std::size_t i = 0; i < second.length(); i++) {
		if (std::isdigit(second[i]) == 0)
			return (false);
	}
	time.tm_sec = std::atoi(second.c_str());
	return (true);
}

bool	isObsoleteDate(const String &date, std::tm &time) {
	if (isRFC850Date(date, time) == false)
		return (false);
	if (isASCTimeDate(date, time) == false)
		return (false);
	return (true);
}

bool	isRFC850Date(const String &date, std::tm &time) {
	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, ','))
		return (false);
	if (isDayNameOne(str, time) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (!String::getline(stream, str, ' '))
		return (false);
	if (isDateTwo(str, time) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isTimeOfDay(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str != "GMT")
		return (false);
	return (true);
}

bool	isDayNameOne(const String &day, std::tm &time) {
	static const String	days[NUM_OF_DAYS] = {
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

bool	isDateTwo(const String &date, std::tm &time) {
	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, '-'))
		return (false);
	if (isDay(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '-'))
		return (false);
	if (isMonth(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str.length() != 2)
		return (false);
	for (std::size_t i = 0; i < str.length(); i++) {
		if (std::isdigit(str[i]) == 0)
			return (false);
	}

	std::time_t	currTime = std::time(NULL);
	std::tm		*currTimeStruct = std::gmtime(&currTime);

	time.tm_year = (currTimeStruct->tm_year / 100) * 100 + std::atoi(str.c_str());
	return (true);
}

bool	isASCTimeDate(const String &date, std::tm &time) {
	std::stringstream	stream(date);
	String				str;
	String				buffer;

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isDayName(str, time) == false)
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
	if (isDateThree(buffer, time) == false)
		return (false);

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isTimeOfDay(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (isYear(str, time) == false)
		return (false);
	return (true);
}

bool	isDateThree(const String &date, std::tm &time) {
	std::stringstream	stream(date);
	String				str;

	if (!String::getline(stream, str, ' '))
		return (false);
	if (isMonth(str, time) == false)
		return (false);

	if (!String::getline(stream, str, '\0'))
		return (false);
	if (str[0] == ' ')
		str = str.substr(1);
	for (std::size_t i = 0; i < str.length(); i++) {
		if (std::isdigit(str[i]) == 0)
			return (false);
	}
	time.tm_mday = std::atoi(str.c_str());
	return (true);
}
