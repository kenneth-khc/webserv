/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 22:25:18 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 22:58:19 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <vector>
#include "String.hpp"

// TODO: maybe use bitmasks to check for valid Contexts? would that be better?
enum Context
{
	NONE,
	GLOBAL,
	HTTP,
	SERVER,
	CGI_SCRIPT,
	LOCATION
};

Context	contextify(const String&);
String	stringifyContext(Context);

// HACK: because there are no initializer lists in C++98, initializing a vector
//		 of Contexts is a pain in the ass
//		 this is a hack to overload the comma operator to return a vector of
//		 Contexts
//		 (C1, C2) would return a vector of [C1, C2]
//		 ([C1, C2], C3) would return a vector of [C1, C2, C3]
//
std::vector<Context>	operator,(Context, Context);
std::vector<Context>	operator,(std::vector<Context>, Context);

#endif
