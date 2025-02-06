/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:41:42 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 23:14:18 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include "MessageBody.hpp"
#include <iostream>
#include <debugUtils.hpp>
#include <utility>
#include "Request.hpp"

size_t	findAfter(const std::string& str, const std::string& substr)
{
	std::string::size_type	pos = str.find(substr);
	if (pos != str.npos)
	{
		return pos + substr.length();
	}
	else
	{
		return pos;
	}
}

bool	consume(std::string& message, const std::string& expected)
{
	if (message.find(expected) == 0)
	{
		message = message.substr(expected.length());
		return true;
	}
	else
	{
		throw std::exception();
		return false;
	}
}

bool	skipTransportPadding(std::string& message)
{
	for (size_t i = 0; i < message.size(); ++i)
	{
		if (message[i] == '\r' && message[i+1] == '\n')
		{
			message = message.substr(i+2);
			return true;
		}
		if (!std::isspace(message[i]))
		{
			return false;
		}
	}
	return false;
}

#include "Optional.hpp"
using std::pair;
using std::string;

Optional< pair<string,string> >	extractKeyValue(std::string& message)
{
	std::string	key;
	std::string	value;

	std::string::size_type	equalPos = message.find('=');
	if (equalPos != message.npos)
	{
		key = message.substr(0, equalPos);
		std::string::size_type	end = message.find_first_of(";\r\n");
		value = message.substr(equalPos+1, end-equalPos-1);
		if (value[0] == '"' && value[value.length()-1] == '"')
		{
			value = value.substr(1, value.length()-2);
		}
		return makeOptional(std::make_pair(key, value));
	}
	return makeNone<std::pair<string,string> >();
}

/* Extract a MessageBody from a Request */
MessageBody::MessageBody(const Request& request):
body(request.messageBody)
{
	const std::string&	field = request.find<std::string>("Content-Type");
	if (field == "application/x-www-form-urlencoded")
	{
		contentType = "application/x-www-form-urlencoded";
	}
	else // "multipart/form-data"
	{
		contentType = field.substr(0, field.find(';'));
		const std::string	boundary = field.substr(findAfter(field, "boundary="));
		dashBoundary = "--" + boundary;
		closeBoundary =  boundary + "--";
		parseMultipartFormData();
	}
}

void	MessageBody::parseMultipartFormData()
{
	while (!body.consume(closeBoundary))
	{
		body.consume(dashBoundary);

	}
}
