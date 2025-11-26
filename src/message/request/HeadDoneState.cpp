/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadDoneState.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:25:17 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/21 08:42:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCode.hpp"
#include "Optional.hpp"
#include "contentLength.hpp"
#include "Client.hpp"
#include "ClientBodyTimer.hpp"
#include "Request.hpp"
#include "MessageBodyState.hpp"
#include "DoneState.hpp"
#include "HeadDoneState.hpp"

/*
	Before deciding to parse the message body, checks the existence and value
	of "Content-Length" or "Transfer-Encoding" headers. "Transfer-Encoding"
	must have the value of "chunked".

	If "Content-Length" or "Transfer-Encoding: chunked" exist, returns the
	next stage (message body), otherwise returns the final stage (done).
*/
RequestState	*HeadDoneState::process(Request &request, Client &client) {
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

	client.timer = new ClientBodyTimer();
	return (new MessageBodyState());
}

int	HeadDoneState::getState(void) const {
	return (RequestState::HEAD_DONE);
}
