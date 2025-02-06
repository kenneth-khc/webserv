/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBodyUtils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:01:06 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 23:13:22 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "MessageBody.hpp"

bool	isCR(char c)
{
	return c == '\r';
}

bool	isLF(char c)
{
	return c == '\n';
}

bool	isCRLF(const std::string& s)
{
	return isCR(s[0]) && isLF(s[1]);
}

bool	isSpace(char c)
{
	return c == ' ';
}

bool	isHTab(char c)
{
	return c == '\t';
}

bool	isWhiteSpace(char c)
{
	return isSpace(c) || isHTab(c);
}

bool	isLinearWhiteSpace(const std::string& s)
{
	return isWhiteSpace(s[0]) || (isCRLF(&s[0]) && isWhiteSpace(s[2]));
}
