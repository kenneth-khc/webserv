/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmptyState.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:20:45 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/29 23:14:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "RequestLineState.hpp"
#include "EmptyState.hpp"

RequestState	*EmptyState::process(Request &request, Client &client) {
	(void)request;

	Optional<String::size_type>	terminatorPos = client.message.find("\r\n");

	if (terminatorPos.exists == false)
		return (this);
	else
		return (new RequestLineState());
}
