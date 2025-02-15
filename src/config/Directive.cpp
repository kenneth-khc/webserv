/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:57:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:11:05 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

Directive::Directive(const String& dname, const String& param, const String& context):
name(dname),
param(param),
enclosingContext(context)
{

}

