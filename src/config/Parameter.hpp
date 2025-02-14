/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parameter.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:33:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:13:08 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <vector>
#include <exception>
#include "String.hpp"

struct	Parameter
{
	std::vector<String>	params;
};

struct	InvalidParameter : public std::exception
{
	String	parameter;

	InvalidParameter(const String& str):
	parameter(str) { }

	~InvalidParameter() throw() { }
};

#endif
