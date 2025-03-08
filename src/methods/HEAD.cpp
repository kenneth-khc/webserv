/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HEAD.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:16:13 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/28 09:18:22 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Driver.hpp"
#include "debugUtils.hpp"

void	Driver::head(Response&, const Request&) const
{
	dbg::println("!!!!!!!!!!!!!!!! HEADING !!!!!!!!!!!!!!!!");
}
