/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 07:29:48 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 07:29:54 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool	toBool(const String& str)
{
	if (str == "on")
	{
		return true;
	}
	else
	{
		return false;
	}
}
