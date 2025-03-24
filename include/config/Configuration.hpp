/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:10:19 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/04 22:40:23 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "Optional.hpp"
#include "Directive.hpp"

struct	Configuration
{
	Configuration();
	~Configuration();

	std::multimap<String, Directive*>	directives;

	void				add(const Directive&);
	void				add(Directive*);
	Optional<Directive>	find(const String&) const;
	void				display() const;
	/*const Directive*	get(const String&) const;*/
	const Directive&	get(const String&) const;
	void				assertHasDirective(const String&) const;
};

#endif
