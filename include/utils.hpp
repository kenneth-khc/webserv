/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:26:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/19 17:29:50 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>

void	error(const std::string& errmsg);

template <typename Type>
std::string	toString(Type const& t)
{
	std::stringstream	ss;
	ss << t;
	return ss.str();
}

#endif
