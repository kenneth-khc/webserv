/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:55:57 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/06 18:25:25 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATE_HPP
# define DATE_HPP

# include <string>
# include <ctime>

# define NUM_OF_DAYS 7
# define NUM_OF_MONTHS 12

bool	isDate(const std::string &date, std::tm &time);
bool	isHTTPDate(const std::string &date, std::tm &time);
bool	isIMFFixDate(const std::string &date, std::tm &time);
bool	isDayName(const std::string &day, std::tm &time);
bool	isDateOne(const std::string &date, std::tm &time);
bool	isDay(const std::string &day, std::tm &time);
bool	isMonth(const std::string &month, std::tm &time);
bool	isYear(const std::string &year, std::tm &time);
bool	isTimeOfDay(const std::string &timeOfDay, std::tm &time);
bool	isHour(const std::string &hour, std::tm &time);
bool	isMinute(const std::string &minute, std::tm &time);
bool	isSecond(const std::string &second, std::tm &time);
bool	isObsoleteDate(const std::string &date, std::tm &time);
bool	isRFC850Date(const std::string &date, std::tm &time);
bool	isDayNameOne(const std::string &day, std::tm &time);
bool	isDateTwo(const std::string &date, std::tm &time);
bool	isASCTimeDate(const std::string &date, std::tm &time);
bool	isDateThree(const std::string &date, std::tm &time);

#endif
