/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadDoneState.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:25:17 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/31 08:12:15 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCode.hpp"
#include "Optional.hpp"
#include "contentLength.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "MessageBodyState.hpp"
#include "DoneState.hpp"
#include "HeadDoneState.hpp"

RequestState	*HeadDoneState::process(Request &request, Client &client) {
	client.timer &= ~Client::CLIENT_HEADER;

	Optional<String>	contentLength = request["Content-Length"];
	Optional<String>	transferEncoding = request["Transfer-Encoding"];

	if (transferEncoding.exists == true)
	{
		if (contentLength.exists == true)
		{
			request.headers.erase("content-length");
		}
		transferEncoding.value = transferEncoding.value.lower();
		if (transferEncoding.value.find("chunked").exists == false)
		{
			throw BadRequest400();
		}
	}
	else if (contentLength.exists == true)
	{
		if (isContentLengthHeader(contentLength.value) == false)
		{
			throw BadRequest400();
		}
	}
	else
		return (new DoneState());

	client.timer |= Client::CLIENT_BODY;
	return (new MessageBodyState());
}

int	HeadDoneState::getState(void) const {
	return (RequestState::HEAD_DONE);
}
