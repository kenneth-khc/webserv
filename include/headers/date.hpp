/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:55:57 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:06:54 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATE_HPP
# define DATE_HPP

# include <ctime>
# include "String.hpp"

# define NUM_OF_DAYS 7
# define NUM_OF_MONTHS 12

bool	isDateHeader(const String &date);
bool	isHTTPDate(std::tm &time, const String &date);
bool	isIMFFixDate(std::tm &time, const String &date);
bool	isDayName(std::tm &time, const String &day);
bool	isDateOne(std::tm &time, const String &date);
bool	isDay(std::tm &time, const String &day);
bool	isMonth(std::tm &time, const String &month);
bool	isYear(std::tm &time, const String &year);
bool	isTimeOfDay(std::tm &time, const String &timeOfDay);
bool	isHour(std::tm &time, const String &hour);
bool	isMinute(std::tm &time, const String &minute);
bool	isSecond(std::tm &time, const String &second);
bool	isObsoleteDate(std::tm &time, const String &date);
bool	isRFC850Date(std::tm &time, const String &date);
bool	isDayNameOne(std::tm &time, const String &day);
bool	isDateTwo(std::tm &time, const String &date);
bool	isASCTimeDate(std::tm &time, const String &date);
bool	isDateThree(std::tm &time, const String &date);

#endif
