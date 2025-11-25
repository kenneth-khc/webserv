/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:08:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:37:59 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_HPP
# define TIME_HPP

# include <ctime>
# include "String.hpp"

//	Handle most time-related operations
namespace Time {
	std::time_t	getTimeSinceEpoch(void);
	std::time_t	getTimeSinceEpoch(std::tm &time);
	std::time_t	convertToGMT(const std::time_t &time);
	String		printHTTPDate(void);
	String		printHTTPDate(const struct timespec &time);
	double		difftime(const std::time_t timeOne, const std::time_t timeTwo);
	String		printAutoindexDate(const struct timespec &time);
}

#endif
