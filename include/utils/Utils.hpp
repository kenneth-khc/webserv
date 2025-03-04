/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:04:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/05 13:22:47 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "String.hpp"
#include <sstream>

template <typename ToType>
ToType	to(const String& str)
{
	std::istringstream	ss(str);
	ToType				converted;
	ss >> converted;
	return converted;
}

#endif
