/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DoneState.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 02:34:13 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/30 04:27:31 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "DoneState.hpp"

RequestState	*DoneState::process(Request &request, Client &client) {
	(void)request;

	client.timer &= ~Client::CLIENT_BODY;
	if (client.message.length() > 0)
	{
		client.timer |= Client::CLIENT_HEADER;
	}
	return (this);
}

int	DoneState::getState(void) const {
	return (RequestState::DONE);
}
