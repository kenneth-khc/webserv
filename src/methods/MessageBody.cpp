/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:41:42 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/30 22:43:03 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "MessageBody.hpp"
#include <iostream>
#include <debugUtils.hpp>

MessageBody::MessageBody(const std::string& messageString)
{
	std::cout << messageString << '\n';
	dbg::println("-----------------------------");

}
