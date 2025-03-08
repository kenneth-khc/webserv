/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:57:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/01 19:24:30 by kecheong         ###   ########.fr       */
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
	directives.insert(std::make_pair(dir.name, dir));
}

const Directive&	Directive::getDirective(const String& key)
{
	return directives.find(key)->second;
}

std::vector<Directive>	Directive::getDirectives(const String& key) const
{
	std::vector<Directive>	matchingDirectives;
	DirectiveRange	range = directives.equal_range(key);
	while (range.first != range.second)
	{
		const std::pair<String,Directive>&	entry = *range.first;
		const Directive&					directive = entry.second;
		matchingDirectives.push_back(directive);
		++range.first;
	}
	return matchingDirectives;
}

bool	Directive::hasParameters() const
{
	return !parameters.empty();
}

void	Directive::printParameters() const
{
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		std::cout << parameters[i];
		if (i != parameters.size()-1)
			std::cout << " ";
	}
}

Optional<Directive>	Directive::find(const String& key) const
{
	std::multimap<String,Directive>::const_iterator it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone<Directive>();
	}
	else
	{
		return makeOptional(it->second);
	}
}

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
	else if (str == "global")
	{
		return GLOBAL;
	}
	else if (str == "none")
	{
		return NONE;
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
	else if (ctx == GLOBAL)
	{
		return "global";
	}
	else if (ctx == NONE)
	{
		return "none";
	}
	else
	{
		throw std::exception();
	}
	
}

