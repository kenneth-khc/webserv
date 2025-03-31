/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHeaderState.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:16:29 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 01:13:10 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "ClientHeaderState.hpp"

ClientHeaderState::ClientHeaderState(void) : ClientTimerState() {}

ClientHeaderState::~ClientHeaderState(void) {}

ClientHeaderState::ClientHeaderState(const ClientHeaderState &obj) :
	ClientTimerState(obj)
{}

ClientHeaderState	&ClientHeaderState::operator=(const ClientHeaderState &obj) {
	if (this == &obj)
		return (*this);

	ClientTimerState::operator=(obj);
	return (*this);
}

void	ClientHeaderState::update(const Server &server) {
	if (server.clientHeaderTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + server.clientHeaderTimeoutDuration;
}

bool	ClientHeaderState::isTimeout(const Server &server) const {
	if (server.clientHeaderTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	ClientHeaderState::getState(void) const {
	return (ClientTimerState::CLIENT_HEADER);
}

int	ClientHeaderState::getLogState(void) const {
	return (Logger::CLIENT_HEADER_TIMEOUT);
}

ClientTimerState	*ClientHeaderState::clone(void) const {
	ClientTimerState	*copyState = new ClientHeaderState(*this);

	return (copyState);
}
