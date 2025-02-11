/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:41:42 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 02:04:05 by cteoh            ###   ########.fr       */
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
	Optional<String>	field = request["Content-Type"];
	String				contentTypeField = field.value_or("");

	if (contentTypeField.match("application/x-www-form-urlencoded"))
	{
		parseURLEncoded();
	}
	// TODO: more content types ?
	else if (contentTypeField.match("multipart/form-data"))
	{
		parseMultipartFormData(contentTypeField);
	}
}

void	MessageBody::parseURLEncoded()
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

void	MessageBody::parseMultipartFormData(String& field)
{
	contentType = field.consumeUntil(";").value_or("");
	Optional<String::size_type>	boundaryPos = field.findAfter("boundary=");
	if (!boundaryPos.exists)
	{
		throw BadRequest400();
	}
	const std::string	boundary = field.substr(boundaryPos.value);
	dashBoundary = "--" + boundary;
	closeBoundary =  dashBoundary + "--";

	while (!body.match(closeBoundary))
	{
		try
		{
			MessageBodyPart	part;
			body.consume(dashBoundary);
			body.consumeIfUntil(Predicate(" \t"), "\r\n");
			body.consume("\r\n");
			while (!body.match("\r\n"))
			{
				Optional<String>	headerLine = body.consumeUntil("\r\n");
				if (headerLine.exists)
				{
					part.extractKeyValue(headerLine.value);
				}
				body.consume("\r\n");
			}
			// end of headers found
			body.consume("\r\n");
			part.content = body.consumeUntil("\r\n" + dashBoundary).value_or("");
			parts.push_back(part);
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

// TODO: what to do with the stored header values?
void	MessageBodyPart::extractKeyValue(const String& headerLine)
{
	std::vector<String>	tokens = headerLine.split(":; ");

	if (tokens[0] == "Content-Disposition")
	{
		for (size_t i = 1; i < tokens.size(); ++i)
		{
			Optional<String::size_type>	equalPos = tokens[i].find("=");
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

