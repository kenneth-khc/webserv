/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:55:57 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:35:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATE_HPP
# define DATE_HPP

# include <ctime>
# include "String.hpp"

# define NUM_OF_DAYS 7
# define NUM_OF_MONTHS 12

bool	isDateHeader(const String &date);
bool	isHTTPDate(const String &date, std::tm &time);
bool	isIMFFixDate(const String &date, std::tm &time);
bool	isDayName(const String &day, std::tm &time);
bool	isDateOne(const String &date, std::tm &time);
bool	isDay(const String &day, std::tm &time);
bool	isMonth(const String &month, std::tm &time);
bool	isYear(const String &year, std::tm &time);
bool	isTimeOfDay(const String &timeOfDay, std::tm &time);
bool	isHour(const String &hour, std::tm &time);
bool	isMinute(const String &minute, std::tm &time);
bool	isSecond(const String &second, std::tm &time);
bool	isObsoleteDate(const String &date, std::tm &time);
bool	isRFC850Date(const String &date, std::tm &time);
bool	isDayNameOne(const String &day, std::tm &time);
bool	isDateTwo(const String &date, std::tm &time);
bool	isASCTimeDate(const String &date, std::tm &time);
bool	isDateThree(const String &date, std::tm &time);

#endif
