/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RDP.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/13 05:49:38 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RDP_HPP
#define RDP_HPP

#include <fstream>
#include "String.hpp"
#include "Token.hpp"

struct	RDP
{
	RDP(const char* fileName);

	std::ifstream	configFile;
	const String	config;
	String			input;
	size_t			lineOffset;
	size_t			charOffset;


};

#endif
