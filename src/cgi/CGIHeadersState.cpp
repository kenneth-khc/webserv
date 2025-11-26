/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHeadersState.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:03:16 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/27 06:25:15 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "ErrorCode.hpp"
#include "CGI.hpp"
#include "CGIHeadersState.hpp"
#include "CGIMessageBodyState.hpp"

/*
	Checks the CGI output for "\r\n\r\n" (CRLF CRLF) or "\n\n" (NL NL) which
	indicates the end of header, for example:

		Content-Type:text/html\n
		Status:200\n\n

	Parses and stores the header in a map.

	Returns the next stage (message body) for processing if parsing completes.
*/
CGIOutputState	*CGIHeadersState::parse(
Response &response,
String &output,
ssize_t bytes)
{
	String						delimiter = "\r\n\r\n";
	Optional<String::size_type>	delimiterPos = output.find(delimiter);

	if (delimiterPos.exists == false) {
		delimiter = "\n\n";
		delimiterPos = output.find(delimiter);
		if (delimiterPos.exists == false && bytes == 0)
			throw InternalServerError500();
		else if (delimiterPos.exists == false)
			return (this);
	}

	String							headerPart = output.substr(0, delimiterPos.value);
	std::vector<String>				headers;
	std::vector<String>				existingFieldNames;
	std::multimap<String, String>	validHeaders;

	output.erase(0, delimiterPos.value + delimiter.length());
	if (delimiter == "\r\n\r\n")
		headers = headerPart.split("\r\n");
	else
		headers = headerPart.split("\n");

	for (std::vector<String>::const_iterator it = headers.begin(); it != headers.end(); it++) {
		Optional<String::size_type>	pos = it->find(":");

		if (pos.exists == false)
			throw InternalServerError500();

		String	fieldName = it->substr(0, pos.value);
		String	fieldValue = it->substr(pos.value + 1);

		if (fieldValue.length() == 0)
			continue ;

		String	trim = fieldName.trim(" \t\n");

		if (trim.length() != fieldName.length())
			throw InternalServerError500();

		existingFieldNames.push_back(fieldName.lower());
		validHeaders.insert(std::make_pair(fieldName.title(), fieldValue.trim(" \t\n")));
	}

	int	total = 0;
	for (int i = 0; i < NUM_OF_CGI_FIELDS; i++) {
		int	count = std::count(existingFieldNames.begin(), existingFieldNames.end(), CGI::cgiFields[i]);

		if (count > 1)
			throw InternalServerError500();
		total += count;
	}
	if (total == 0)
		throw InternalServerError500();

	std::multimap<String, String>::iterator	it;
	std::stringstream						stream;
	String									str;
	int										statusCode;

	it = validHeaders.find("Status");
	if (it == validHeaders.end())
		response.setStatusCode(Response::OK);
	else {
		stream << it->second;
		stream >> statusCode;
		response.setStatusCode(statusCode);
		String::getline(stream, str);
		response.reasonPhrase = str.trim(" ");
		validHeaders.erase(it);
	}

	response.headers = validHeaders;
	return (new CGIMessageBodyState());
}
