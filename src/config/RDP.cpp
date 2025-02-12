/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RDP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:45:56 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 22:28:04 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include "String.hpp"
#include "RDP.hpp"

RDP::RDP(const char *fileName):
configFile(fileName),
config(configFile),
input(config),
lineOffset(0),
charOffset(0)
{

}

