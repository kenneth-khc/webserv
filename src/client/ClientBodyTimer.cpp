/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientBodyTimer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:18:09 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:58:54 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "ClientBodyTimer.hpp"

ClientBodyTimer::ClientBodyTimer(void) : Timer() {}

ClientBodyTimer::~ClientBodyTimer(void) {}

ClientBodyTimer::ClientBodyTimer(const ClientBodyTimer &obj) :
	Timer(obj)
{}

ClientBodyTimer	&ClientBodyTimer::operator=(const ClientBodyTimer &obj) {
	if (this == &obj)
		return (*this);

	Timer::operator=(obj);
	return (*this);
}

void	ClientBodyTimer::update(void) {
	if (Server::clientBodyTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + Server::clientBodyTimeoutDuration;
}

bool	ClientBodyTimer::isTimeout(void) const {
	if (Server::clientBodyTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	ClientBodyTimer::getType(void) const {
	return (Timer::CLIENT_BODY);
}

int	ClientBodyTimer::getLogType(void) const {
	return (Logger::CLIENT_BODY_TIMEOUT);
}

Timer	*ClientBodyTimer::clone(void) const {
	Timer	*copyState = new ClientBodyTimer(*this);

	return (copyState);
}
