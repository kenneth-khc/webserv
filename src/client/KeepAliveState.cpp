/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeepAliveState.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:59:44 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 01:12:33 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "KeepAliveState.hpp"

KeepAliveState::KeepAliveState(void) : ClientTimerState() {}

KeepAliveState::~KeepAliveState(void) {}

KeepAliveState::KeepAliveState(const KeepAliveState &obj) :
	ClientTimerState(obj)
{}

KeepAliveState	&KeepAliveState::operator=(const KeepAliveState &obj) {
	if (this == &obj)
		return (*this);

	ClientTimerState::operator=(obj);
	return (*this);
}

void	KeepAliveState::update(const Server &server) {
	if (server.keepAliveTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + server.keepAliveTimeoutDuration;
}

bool	KeepAliveState::isTimeout(const Server &server) const {
	if (server.keepAliveTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	KeepAliveState::getState(void) const {
	return (ClientTimerState::KEEP_ALIVE);
}

int	KeepAliveState::getLogState(void) const {
	return (Logger::KEEP_ALIVE_TIMEOUT);
}

ClientTimerState	*KeepAliveState::clone(void) const {
	ClientTimerState	*copyState = new KeepAliveState(*this);

	return (copyState);
}
