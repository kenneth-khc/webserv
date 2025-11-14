/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:10:19 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/05 22:15:03 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "Optional.hpp"
#include "Directive.hpp"

/* A representation of the config file in code. The layer here corresponds to
 * the global scope of the config file itself.
 * 
 * A multimap holds all the directives, which in turn the directives themselves
 * hold another multimap of directives, forming a hierarchical relationship 
 *
 * A Driver object takes in a Configuration and reads from it to configure
 * specific settings related to HTTP or the web server itself. */

class	Configuration
{
public:
	Configuration();
	~Configuration();

	typedef std::multimap<String, Directive*>	DirectiveMappings;
	typedef DirectiveMappings::iterator			DirectiveMappingsIter;
	typedef DirectiveMappings::const_iterator	DirectiveMappingsConstIter;

	void					add(Directive*);
	const Directive&		get(const String&) const;
	Optional<Directive*>	getDirective(const String&) const;
	void					display() const;

	DirectiveMappings	directives;

private:
	void	print(const Directive&,
				  int indent = 0,
				  int charStart = 0,
				  bool lastRow = false) const;
};

#endif
