/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:32:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 22:33:22 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <vector>
#include "String.hpp"
#include "Context.hpp"

struct	Directive
{
	// Name of the current directive
	String					name;
	// Parameters of the current directive
	std::vector<String>		parameters;
	/*Context				context;*/
	// The context this directive is in
	Context					enclosingContext;
	// The directives within the directive
	std::vector<Directive>	directives;

	Directive();
	Directive(const String&, const std::vector<String>&, Context);
	/*Directive(const String&, const std::vector<String>&, Context, Context);*/

	void	addDirective(const Directive&);
};

#endif
