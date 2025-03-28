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

PathHandler::PathHandler(const String& prefix):
	prefix(prefix)
{
	std::cout << this->prefix << '\n';
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

String	PathHandler::join(const String& lhs, const String& rhs) const
{
	if (isAbsolute(rhs))
	{
		return rhs;
	}
	else
	{
		return lhs + rhs;
	}
}
