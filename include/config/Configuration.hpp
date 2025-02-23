/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:10:19 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 18:31:57 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <map>
#include "Directive.hpp"

struct	Configuration
{
	Configuration();

	std::multimap<String,Directive>	directives;

	void		add(const Directive&);
	Directive	find(const String&);
	void		display() const;
	void		printMatchingElements(const String& key);
};

#endif
