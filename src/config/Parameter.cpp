/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parameter.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:37:18 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:04:34 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parameter.hpp"
#include "Diagnostic.hpp"

Parameter::Parameter(const char* value):
	value(value),
	diagnostic()
{
}

Parameter::Parameter(const String& value):
	value(value),
	diagnostic()
{
}

Parameter::Parameter(const String& value, const Diagnostic& diagnostic):
	value(value),
	diagnostic(diagnostic)
{
}

bool	Parameter::operator==(const Parameter& rhs) const
{
	return this->value == rhs.value;
}

bool	Parameter::operator<(const Parameter& rhs) const
{
	return this->value < rhs.value;
}

char&	Parameter::operator[](size_t index)
{
	return value[index];
}

const char&	Parameter::operator[](size_t index) const
{
	return value[index];
}

Parameter::operator String()
{
	return value;
}

Parameter::operator const String() const
{
	return value;
}
