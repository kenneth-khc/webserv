/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DoneState.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 02:34:13 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/31 23:32:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ClientHeaderState.hpp"
#include "DoneState.hpp"

RequestState	*DoneState::process(Request &request, Client &client) {
	(void)request;

	if (client.message.length() > 0)
	{
		client.timer = new ClientHeaderState();
	}
	return (this);
}

int	DoneState::getState(void) const {
	return (RequestState::DONE);
}
