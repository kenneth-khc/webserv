/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DoneState.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 02:34:13 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/29 13:31:37 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ClientHeaderTimer.hpp"
#include "DoneState.hpp"

RequestState	*DoneState::process(Request &request, Client &client) {
	(void)request;

	if (client.timer == 0 && client.message.length() > 0)
	{
		client.timer = new ClientHeaderTimer();
	}
	return (this);
}

int	DoneState::getState(void) const {
	return (RequestState::DONE);
}
