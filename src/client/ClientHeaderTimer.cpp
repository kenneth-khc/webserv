/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHeaderTimer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:16:29 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/05 10:25:03 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "ClientHeaderTimer.hpp"

ClientHeaderTimer::ClientHeaderTimer(void) : Timer() {
	this->update();
}

ClientHeaderTimer::~ClientHeaderTimer(void) {}

ClientHeaderTimer::ClientHeaderTimer(const ClientHeaderTimer &obj) :
	Timer(obj)
{}

ClientHeaderTimer	&ClientHeaderTimer::operator=(const ClientHeaderTimer &obj) {
	if (this == &obj)
		return (*this);

	Timer::operator=(obj);
	return (*this);
}

void	ClientHeaderTimer::update(void) {
	if (Server::clientHeaderTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + Server::clientHeaderTimeoutDuration;
}

bool	ClientHeaderTimer::isTimeout(void) const {
	if (Server::clientHeaderTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	ClientHeaderTimer::getType(void) const {
	return (Timer::CLIENT_HEADER);
}

int	ClientHeaderTimer::getLogType(void) const {
	return (Logger::CLIENT_HEADER_TIMEOUT);
}

Timer	*ClientHeaderTimer::clone(void) const {
	Timer	*copyState = new ClientHeaderTimer(*this);

	return (copyState);
}
