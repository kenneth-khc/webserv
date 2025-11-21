/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersState.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:01:55 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/21 04:37:49 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "terminalValues.hpp"
#include "ErrorCode.hpp"
#include "headers.hpp"
#include "Client.hpp"
#include "HeadDoneState.hpp"
#include "HeadersState.hpp"

/*
	Checks the client socket's message for "\r\n" (CRLF) which indicates the
	end a header, for example:

		Host: localhost\r\n

	Parses and stores the header in a map.

	If during parsing "\r\n\r\n" (CRLF CRLF) is encountered, it means the end
	of header section.

		Host: localhost\r\n
		Connection: keep-alive\r\n\r\n

	Returns the next stage ("head done").
*/
RequestState	*HeadersState::process(Request &request, Client &client) {
	String	&message = client.message;

	if (message.find("\r\n").exists == false)
		return (this);

	Optional<String::size_type>	headerLineTerminator;
	const String				optionalWhiteSpaces = " \t";

	while (true) {
		if (message.starts_with("\r\n\r\n") == true)
			break ;

		headerLineTerminator = message.find("\r\n", 2);
		if (headerLineTerminator.exists == false)
			return (this);

		if (message.find(':').exists == false)
			throw BadRequest400();

		std::stringstream	stream(message.substr(2, headerLineTerminator.value - 2));
		String				str;
		String				fieldName;

		String::getline(stream, str, ':');
		if (isToken(str) == false)
			throw BadRequest400();
		fieldName = str;

		if (!String::getline(stream, str, '\0'))
			return (this);
		if (str.find("\r\n ").exists == true || str.find("\r\n\t").exists == true)
			throw BadRequest400("Invalid use of obsolete line folding in field value.");

		str = str.trim(optionalWhiteSpaces);

		if (isFieldValue(str) == false)
			throw BadRequest400();
		fieldName = fieldName.lower();
		if (checkMandatoryHeaders(fieldName, str) == false)
			throw BadRequest400();
		request.insert(fieldName, str);

		message.erase(0, headerLineTerminator.value);
	}
	message.erase(0, 4);

	if (request["Host"].exists == false)
		throw BadRequest400();

	delete client.timer;
	client.timer = 0;
	return (new HeadDoneState());
}

int	HeadersState::getState(void) const {
	return (RequestState::HEADERS);
}

bool	isFieldVisibleCharacter(const unsigned char &character) {
	if (isVisibleCharacter(character) == true)
		return (true);
	if (isObsoleteText(character) == true)
		return (true);
	return (false);
}

bool	isFieldContent(const String &line) {
	const String		values = " \t";
	String::size_type	length = line.length();

	if (length == 0 || values.find(line[length - 1]).exists == true)
		return (false);

	for (String::size_type i = 0; i < line.length(); i++) {
		if (isFieldVisibleCharacter(line[i]) == true)
			continue ;
		if (values.find(line[i]).exists == true)
			continue ;
		return (false);
	}
	return (true);
}

bool	isFieldValue(const String &line) {
	if (line.length() == 0)
		return (true);
	if (isFieldContent(line) == false)
		return (false);
	return (true);
}
