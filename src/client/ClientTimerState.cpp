/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientTimerState.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:55:21 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 00:57:56 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientTimerState.hpp"

ClientTimerState::ClientTimerState(void) : timeout(Time::getTimeSinceEpoch()) {}

ClientTimerState::~ClientTimerState(void) {}

ClientTimerState::ClientTimerState(const ClientTimerState &obj) : timeout(obj.timeout) {}

ClientTimerState	&ClientTimerState::operator=(const ClientTimerState &obj) {
	if (this == &obj)
		return (*this);

	this->timeout = obj.timeout;
	return (*this);
}

const std::time_t	&ClientTimerState::getTimeoutTime(void) const {
	return (timeout);
}
