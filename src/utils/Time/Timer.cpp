/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:42:23 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:43:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Timer.hpp"

Timer::Timer(void) : timeout(Time::getTimeSinceEpoch()) {}

Timer::~Timer(void) {}

Timer::Timer(const Timer &obj) : timeout(obj.timeout) {}

Timer	&Timer::operator=(const Timer &obj) {
	if (this == &obj)
		return (*this);

	this->timeout = obj.timeout;
	return (*this);
}

const std::time_t	&Timer::getTimeoutTime(void) const {
	return (timeout);
}
