/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientBodyState.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:18:09 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 01:13:37 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Server.hpp"
#include "ClientBodyState.hpp"

ClientBodyState::ClientBodyState(void) : ClientTimerState() {}

ClientBodyState::~ClientBodyState(void) {}

ClientBodyState::ClientBodyState(const ClientBodyState &obj) :
	ClientTimerState(obj)
{}

ClientBodyState	&ClientBodyState::operator=(const ClientBodyState &obj) {
	if (this == &obj)
		return (*this);

	ClientTimerState::operator=(obj);
	return (*this);
}

void	ClientBodyState::update(const Server &server) {
	if (server.clientBodyTimeoutDuration <= 0)
		return ;

	this->timeout = Time::getTimeSinceEpoch() + server.clientBodyTimeoutDuration;
}

bool	ClientBodyState::isTimeout(const Server &server) const {
	if (server.clientBodyTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timeout)
		return (true);
	else
		return (false);
}

int	ClientBodyState::getState(void) const {
	return (ClientTimerState::CLIENT_BODY);
}

int	ClientBodyState::getLogState(void) const {
	return (Logger::CLIENT_BODY_TIMEOUT);
}

ClientTimerState	*ClientBodyState::clone(void) const {
	ClientTimerState	*copyState = new ClientBodyState(*this);

	return (copyState);
}
