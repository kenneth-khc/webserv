/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:37:36 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/28 21:55:21 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "PathHandler.hpp"

PathHandler::PathHandler():
	prefix()
{
}

PathHandler::PathHandler(const String& prefix):
	prefix(prefix)
{
	std::cout << "PathHandler set with |" << this->prefix << "|\n";
}

void	PathHandler::setPrefix(const String& prefix)
{
	this->prefix = prefix;
}

bool	PathHandler::isAbsolute(const String& path) const
{
	if (path[0] == '/')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool	PathHandler::isRelative(const String& path) const
{
	return !isAbsolute(path);
}

String	PathHandler::normalize(const String& path) const
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

String	PathHandler::resolveWithPrefix(const String& path) const
{
	if (isAbsolute(path))
	{
		return path;
	}
	else
	{
		if (prefix.back() == '/')
		{
			return prefix + path;
		}
		else
		{
			return prefix + '/' + path;
		}
	}
}

String	PathHandler::resolve(const String& lhs, const String& rhs) const
{
	return lhs + rhs;
}
