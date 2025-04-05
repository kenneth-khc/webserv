/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGITimer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:11:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/05 10:24:30 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "CGITimer.hpp"

CGITimer::CGITimer(void) : Timer() {
	this->update();
}

CGITimer::~CGITimer(void) {}

CGITimer::CGITimer(const CGITimer &obj) :
	Timer(obj)
{}

CGITimer	&CGITimer::operator=(const CGITimer &obj) {
	if (this == &obj)
		return (*this);

	Timer::operator=(obj);
	return (*this);
}

void	CGITimer::update(void) {
	if (Server::cgiTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + Server::cgiTimeoutDuration;
}

bool	CGITimer::isTimeout(void) const {
	if (Server::cgiTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	CGITimer::getType(void) const {
	return (Timer::CGI);
}

int	CGITimer::getLogType(void) const {
	return (-1);
}

Timer	*CGITimer::clone(void) const {
	Timer	*copyState = new CGITimer(*this);

	return (copyState);
}
