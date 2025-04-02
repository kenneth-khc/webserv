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

Parameter::Parameter(const String& value, const Diagnostic& diagnostic):
	value(value),
	diagnostic(diagnostic)
{

}

Parameter::operator String()
{
	return value;
}

Parameter::operator const String() const
{
	return value;
}
