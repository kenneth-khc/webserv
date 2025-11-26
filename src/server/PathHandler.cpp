/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:37:36 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/21 08:22:49 by cteoh            ###   ########.fr       */
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
