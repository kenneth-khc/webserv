/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:41:42 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/08 06:12:07 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include <utility>
#include "MessageBody.hpp"
#include "ErrorCode.hpp"
#include "Request.hpp"
#include "String.hpp"

/* Extract a MessageBody from a Request */
MessageBody::MessageBody(const Request& request):
body(request.messageBody)
{
	String	contentTypeField = request.find<std::string>("Content-Type");
	if (contentTypeField == "application/x-www-form-urlencoded")
	{
		contentType = "application/x-www-form-urlencoded";
		std::vector<String>	keyValuePairs = body.split("&");
		for (std::vector<String>::iterator it = keyValuePairs.begin();
			 it != keyValuePairs.end(); ++it)
		{
			std::vector<String>	splitted = it->split("=");
			String&	key = splitted[0];
			String&	value = splitted[1];
			urlEncodedKeyValues.insert(std::make_pair(key, value));
		}
	}
	// TODO: more content types ?
	else // assume this is "multipart/form-data"
	{
		contentType = contentTypeField.consumeUntil(String(";"));
		Optional<String::size_type>	boundaryPos = contentTypeField.findAfter("boundary=");
		// TODO: check this properly
		const std::string	boundary = contentTypeField.substr(boundaryPos.value);
		dashBoundary = "--" + boundary;
		closeBoundary =  dashBoundary + "--";
		parseMultipartFormData();
	}
}

void	MessageBodyChunk::extractKeyValue(const String& headerLine)
{
	Optional<String::size_type>	colonPos = headerLine.find(String(":"));

	std::vector<String>	tokens = headerLine.split(":; ");
	if (tokens[0] == "Content-Disposition")
	{
		for (size_t i = 1; i < tokens.size(); ++i)
		{
			Optional<String::size_type>	equalPos = tokens[i].find(String("="));
			if (equalPos.exists)
			{
				std::vector<String>	splitted = tokens[i].split("=");
				String&	key = splitted[0];
				String&	value = splitted[1];
				if (value[0] == '"' && value[value.size()-1] == '"')
				{
					value = value.substr(1, value.size()-2);
				}
				contentDisposition.insert(std::make_pair(key, value));
			}
			else
			{
				contentDisposition.insert(std::make_pair("type", tokens[i]));
			}
		}
	}
	else if (tokens[0] == "Content-Type")
	{
		contentType = tokens[1];
	}
}

void	MessageBody::parseMultipartFormData()
{
	while (!body.match(closeBoundary))
	{
		try
		{
			MessageBodyChunk	chunk;
			body.consume(dashBoundary);
			body.consumeIfUntil(Predicate(" \t"), "\r\n");
			body.consume("\r\n");
			while (!body.match("\r\n"))
			{
				String	headerLine = body.consumeUntil(String("\r\n"));
				// TODO: store header values
				chunk.extractKeyValue(headerLine);
				body.consume("\r\n");
			}
			// end of headers found
			body.consume("\r\n");
			chunk.content = body.consumeUntil(String("\r\n") + dashBoundary);
			chunks.push_back(chunk);
			body.consume("\r\n");
		}
		catch (const std::exception& e)
		{
			throw BadRequest400();
		}
	}
	body.consume(closeBoundary);
	body.consumeIfUntil(Predicate(" \t"), "\r\n");
	body.consume("\r\n");
}
