/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineState.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:09:36 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/21 03:50:06 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <ios>
#include <sstream>
#include "Optional.hpp"
#include "ErrorCode.hpp"
#include "terminalValues.hpp"
#include "uri.hpp"
#include "path.hpp"
#include "Client.hpp"
#include "HeadersState.hpp"
#include "RequestLineState.hpp"

/*
	Resolves dot segments by looping forward and backward the URI and extract
	the relevant path segment. Example:

		Stage:
		0) /hello/.././there
		1) hello/.././there (Output: /)
		2) /.././there (Output: /hello)
		3) .././there (Output: /hello/)
		4) /./there (Output: )
		5) ./there (Output: /)
		6) there (Output: /)

		Output: /there
*/
String	RequestLineState::removeDotSegments(String input) {
	String				output;
	Optional<String>	temp;

	while (input.length() > 0) {
		if (input[0] == '/') {
			output.push_back(input[0]);
			input.erase(0, 1);
			continue ;
		}

		temp = input.consumeUntil("/");
		if (temp.exists == false) {
			temp.value = input;
			input = "";
		}

		if (temp.value == ".") {
			if (temp.exists == true)
				input.erase(0, 1);
			else
				continue ;
		}
		else if (temp.value == "..") {
			if (output.length() > 0)
				output.erase(output.length() - 1, 1);
			if (output.consumeBackwardsUntil("/").exists == true)
				output.erase(output.length() - 1, 1);
			else
				output = "/";
		}
		else
			output.append(temp.value.c_str(), temp.value.length());
	}
	return (output);
}

/*
	Removes two or more adjacent slashes.
*/
String	RequestLineState::normalize(const String& path)
{
	String			normalized;
	Optional<char>	previous;
	for (size_t i = 0; i < path.size(); ++i)
	{
		if (path[i] == '/' && previous.exists && previous.value == '/')
		{
			continue ;
		}
		else
		{
			normalized += path[i];
			previous = makeOptional(path[i]);
		}
	}
	return normalized;
}

/*
	Decodes percent-encoded values, which are used for representing characters
	that are outside the allowed ones for HTTP, such as ' ' (space). Example:

		%20 where "20" is a hexadecimal value, where converted into decimal
		becomes "32", which corresponds to the ASCII value of ' '.
*/
String	RequestLineState::percentDecodeString(const String &line) {
	String	decodedString;

	for (String::size_type i = 0; i < line.length(); i++) {
		if (line[i] == '%') {
			String::size_type	c;

			std::stringstream(line.substr(i + 1, 2)) >> std::hex >> c;
			decodedString.push_back(c);
			i += 2;
		}
		else if (line[i] == '+')
			decodedString.push_back(' ');
		else
			decodedString.push_back(line[i]);
	}

	return (decodedString);
}

/*
	Checks the client socket's message for "\r\n" (CRLF) which indicates the
	end of request line, for example:

		GET /hello?this=that HTTP/1.1\r\n

	Parses and stores each section of the request line. Additionally, decodes
	percent-encoded values, resolves dot segments (relative path), and
	normalizes/compresses two or more adjacent slashes into one.

	Returns the next stage (headers) for processing if parsing completes.
*/
RequestState	*RequestLineState::process(Request &request, Client &client) {
	String						&message = client.message;
	Optional<String::size_type>	terminatorPos = message.find("\r\n");

	if (terminatorPos.exists == false)
		return (this);

	std::stringstream			stream(message.substr(0, terminatorPos.value));
	String						str;
	String						method;

	String::getline(stream, str, ' ');
	if (isToken(str) == false)
		throw BadRequest400();
	request.method = str;

	String::getline(stream, str, ' ');
	if (isRequestTarget(str) == false)
		throw BadRequest400();
	request.requestTarget = str;

	Optional<String::size_type>	queryPos = str.find("?");
	if (queryPos.exists == true) {
		request.path = str.substr(0, queryPos.value);

		request.query = Optional<String>(str.substr(queryPos.value + 1));
		request.decodedQuery.exists = true;
		request.decodedQuery.value = RequestLineState::percentDecodeString(request.query.value);

		const std::vector<String>	queries = request.decodedQuery.value.split("&");
		for (std::vector<String>::const_iterator it = queries.begin(); it != queries.end(); it++) {
			std::vector<String>	split = it->split("=");

			request.queryPairs.insert(std::make_pair(split[0], split[1]));
		}
	}
	else
		request.path = str;
	request.decodedPath = RequestLineState::removeDotSegments(request.path);
	request.decodedPath = RequestLineState::normalize(request.decodedPath);
	request.decodedPath = RequestLineState::percentDecodeString(request.decodedPath);

	String::getline(stream, str, '/');
	if (str != "HTTP")
		throw BadRequest400();
	String::getline(stream, str, '\0');
	if (str.length() != 3)
		throw BadRequest400();
	if (std::isdigit(str[0]) == 0 || str[1] != '.' || std::isdigit(str[2]) == 0)
		throw BadRequest400();
	request.httpVersion = str;

	message.erase(0, terminatorPos.value);
	return (new HeadersState());
}

int	RequestLineState::getState(void) const {
	return (RequestState::REQUEST_LINE);
}

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const String &line) {
	if (isOriginForm(line) == true)
		return (true);
	if (isAbsoluteForm(line) == true)
		return (true);
	if (isAuthorityForm(line) == true)
		return (true);
	if (isAsteriskForm(line) == true)
		return (true);
	return (false);
}

bool	isOriginForm(const String &line) {
	std::stringstream	stream(line);
	String				str;

	String::getline(stream, str, '?');
	if (isAbsolutePath(str) == false)
		return (false);

	if (!String::getline(stream, str))
		return (true);
	if (isQuery(str) == false)
		return (false);
	return (true);
}

bool	isAbsoluteForm(const String &line) {
	if (isAbsoluteURI(line) == false)
		return (false);
	return (true);
}

bool	isAuthorityForm(const String &line) {
	if (isAuthority(line) == false)
		return (false);
	return (true);
}

bool	isAsteriskForm(const String &line) {
	if (line.length() == 1 && line[0] == '*')
		return (true);
	return (false);
}
