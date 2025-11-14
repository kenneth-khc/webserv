/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configurator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:54:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 18:13:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include <map>
#include <stack>
#include "String.hpp"
#include "Validator.hpp"
#include "Configuration.hpp"

struct	Configurator
{
	Configurator();

	void	support(const String&, const Validator&);
	void	validate(const Directive&,
					 const std::multimap<String,Directive>&);

	void	validate(const Directive*,
					 const std::multimap<String,Directive*>&);

	/* Returns the Validator for the given Directive */
	const Validator&	operator[](const String&) const;

	std::map<String,Validator>	supportedDirectives;

	void	add(Directive*, Configuration&) const;
};

#endif
