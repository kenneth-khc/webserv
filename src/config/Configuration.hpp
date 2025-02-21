/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:10:19 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/21 17:07:36 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include "Directive.hpp"

struct	Configuration
{
	Configuration();

	std::vector<Directive>	directives;

	void	addDirective(const Directive&);
};

#endif
