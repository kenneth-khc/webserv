/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:57:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/22 01:26:00 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "ConfigErrors.hpp"

Directive::Directive()
{

}

Directive::Directive(const String& dname,
					 const std::vector<String>& parameters,
					 Context context):
name(dname),
parameters(parameters),
enclosingContext(context)
{

}

void	Directive::addDirective(const Directive& dir)
{
	directives.push_back(dir);
}

/*Directive::Directive(const String& dname,*/
/*					 const std::vector<String>& parameters,*/
/*					 Context context,*/
/*					 Context enclosing):*/
/*name(dname),*/
/*parameters(parameters),*/
/*context(context),*/
/*enclosing(enclosing)*/
/*{*/
/**/
/*}*/
/**/
Context	contextify(const String& str)
{
	if (str == "http")
	{
		return HTTP;
	}
	else if (str == "server")
	{
		return SERVER;
	}
	else if (str == "location")
	{
		return LOCATION;
	}
	else
	{
		throw InvalidDirective(str);
	}
}

String	stringifyContext(Context ctx)
{
	if (ctx == HTTP)
	{
		return "http";
	}
	else if (ctx == SERVER)
	{
		return "server";
	}
	else if (ctx == LOCATION)
	{
		return "location";
	}
	else
	{
		throw std::exception();
	}
	
}
