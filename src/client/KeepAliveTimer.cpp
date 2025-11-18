/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeepAliveTimer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:59:44 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/05 10:24:41 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "KeepAliveTimer.hpp"

KeepAliveTimer::KeepAliveTimer(void) : Timer() {
	this->update();
}

KeepAliveTimer::~KeepAliveTimer(void) {}

KeepAliveTimer::KeepAliveTimer(const KeepAliveTimer &obj) :
	Timer(obj)
{}

KeepAliveTimer	&KeepAliveTimer::operator=(const KeepAliveTimer &obj) {
	if (this == &obj)
		return (*this);

	Timer::operator=(obj);
	return (*this);
}

void	KeepAliveTimer::update(void) {
	if (Server::keepAliveTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + Server::keepAliveTimeoutDuration;
}

bool	KeepAliveTimer::isTimeout(void) const {
	if (Server::keepAliveTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	KeepAliveTimer::getType(void) const {
	return (Timer::KEEP_ALIVE);
}

int	KeepAliveTimer::getLogType(void) const {
	return (Logger::KEEP_ALIVE_TIMEOUT);
}

Timer	*KeepAliveTimer::clone(void) const {
	Timer	*copyState = new KeepAliveTimer(*this);

	return (copyState);
}
