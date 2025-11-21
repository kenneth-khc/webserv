/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBodyState.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:31:58 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/21 04:34:41 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCode.hpp"
#include "Client.hpp"
#include "MessageBodyState.hpp"
#include "DoneState.hpp"

MessageBodyState::MessageBodyState(void) :
	chunkSize(0),
	bodyLength(0),
	hasChunkSize(false),
	isLastChunk(false)
{}

/*
	If "Content-Length" exists, store the message body in the request object
	until the specified length is reached.

	If "Transfer-Encoding: chunked" exists, decode the encoded data and store
	the message chunks until the "final" chunk is received.

	Returns the final stage (done).
*/
RequestState	*MessageBodyState::process(Request &request, Client &client) {
	String						&message = client.message;
	Optional<String::size_type>	contentLength = request.find< Optional<String::size_type> >("Content-Length");

	if (contentLength.exists == true) {
		String::size_type	remainingBytes = contentLength.value - request.messageBody.length();

		if (message.length() < remainingBytes)
			remainingBytes = message.length();

		request.messageBody.append(message.c_str(), remainingBytes);
		message.erase(0, remainingBytes);

		if (request.messageBody.length() == contentLength.value) {
			delete client.timer;
			client.timer = 0;
			return (new DoneState());
		}
		else
			return (this);
	}

	String						str;
	Optional<String::size_type>	chunkSizeTerminator;
	Optional<String::size_type>	chunkExt;
	Optional<String::size_type>	chunkDataTerminator;

	while (true) {
		if (this->hasChunkSize == false) {
			chunkSizeTerminator = message.find("\r\n");
			if (chunkSizeTerminator.exists == false)
				return (this);

			str = message.substr(0, chunkSizeTerminator.value);

			chunkExt = str.find(";");	// No support for chunk extensions
			if (chunkExt.exists == true)
				str = str.substr(0, chunkExt.value);

			for (String::size_type i = 0; i < str.length(); i++) {
				if (std::isxdigit(str[i]) == 0)
					throw BadRequest400();
			}

			std::stringstream(str) >> std::hex >> this->chunkSize;
			if (this->chunkSize == 0)
				this->isLastChunk = true;

			message.erase(0, chunkSizeTerminator.value + 2);
			this->hasChunkSize = true;
		}

		if (this->hasChunkSize == true) {
			String::size_type	appendableBytes = this->chunkSize;

			if (message.length() < appendableBytes)
				appendableBytes = message.length();

			request.messageBody.append(message.c_str(), appendableBytes);
			message.erase(0, appendableBytes);
			this->chunkSize -= appendableBytes;
			this->bodyLength += appendableBytes;

			if (this->chunkSize > 0)
				return (this);

			chunkDataTerminator = message.find("\r\n");
			if (chunkDataTerminator.exists == false)
				return (this);
			message.erase(0, chunkDataTerminator.value + 2);

			this->hasChunkSize = false;
			if (this->isLastChunk == true)
				break ;
		}
	}
	// No support for chunked trailer section

	request.insert("Content-Length", this->bodyLength);
	request.erase("Transfer-Encoding");
	delete client.timer;
	client.timer = 0;
	return (new DoneState());
}

int	MessageBodyState::getState(void) const {
	return (RequestState::MESSAGE_BODY);
}
