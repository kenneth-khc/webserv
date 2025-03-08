/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 22:28:51 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/24 00:27:59 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "Context.hpp"

// WARN: don't do this at home
std::vector<Context>	operator,(std::vector<Context> contexts, Context rhs)
{
	contexts.push_back(rhs);
	return contexts;
}

// WARN: don't do this at home
std::vector<Context>	operator,(Context lhs, Context rhs)
{
	std::vector<Context>	contexts;
	contexts.push_back(lhs);
	contexts.push_back(rhs);
	return contexts;
}
