/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:08:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:39:25 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_HPP
# define TIME_HPP

# include <ctime>
# include <time.h>
# include "String.hpp"

//	Handle most time-related operations
class Time {
		Time(const Time &obj);
		Time	&operator=(const Time &obj);
	public:
		Time(void);
		~Time(void);

		static std::time_t	getTimeSinceEpoch(std::tm time);
		static std::time_t	convertToGMT(std::time_t time);
		static String		printHTTPDate(void);
		static String		printHTTPDate(const struct timespec &time);
		static double		difftime(const std::time_t timeOne, const std::time_t timeTwo);
};

#endif
