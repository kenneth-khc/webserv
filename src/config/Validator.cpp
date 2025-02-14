/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:06:02 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:09:38 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"
#include "Parameter.hpp"
#include "String.hpp"

Validator::Validator()
{

}

Validator::Validator(bool (*function)(const String&)):
validate(function)
{

}

bool	Validator::operator()(String parameter) const
{
	return validate(parameter);
}

bool	checkIfAbsolutePath(const String& str)
{
	if (str[0] != '/')
	{
		throw InvalidParameter(str);
	}
	return str[0] == '/';
}
