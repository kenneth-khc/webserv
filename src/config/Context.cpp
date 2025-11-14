/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 22:28:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/24 00:27:59 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "Context.hpp"
#include "String.hpp"

Context::Context	Context::from(const String& str)
{
	if (str == "http")
	{
		return HTTP;
	}
	else if (str == "server")
	{
		return SERVER;
	}
	else if (str == "location")
	{
		return LOCATION;
	}
	else if (str == "global")
	{
		return GLOBAL;
	}
	else if (str == "none")
	{
		return NONE;
	}
	else if (str == "CGI_script")
	{
		return CGI_SCRIPT;
	}
	else
	{
		throw std::invalid_argument("invalid context `" + str + '`');
	}
}

String	Context::toString(Context context)
{
	static const String	stringified[] = {
		"none", "global", "http", "server", "CGI_script", "location"
	};

	if (context >= CONTEXT_COUNT)
	{
		throw std::invalid_argument("invalid context to stringify");
	}
	return stringified[context];
}

// WARN: don't do this at home
std::vector<Context::Context>
Context::operator,(Context lhs, Context rhs)
{
	std::vector<Context>	contexts;
	contexts.push_back(lhs);
	contexts.push_back(rhs);
	return contexts;
}

// WARN: don't do this at home
std::vector<Context::Context>
Context::operator,(std::vector<Context> contexts, Context rhs)
{
	contexts.push_back(rhs);
	return contexts;
}
