/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:32:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:10:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <vector>
#include "String.hpp"

struct	Directive
{
	String	name;
	String	param;
	String	enclosingContext;

	Directive(const String&, const String&, const String&);
};

#endif
