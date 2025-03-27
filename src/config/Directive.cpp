/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:57:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/16 01:48:08 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "ConfigErrors.hpp"
#include "VectorInitializer.hpp"
#include <iostream>

Directive::Directive()
{

}

Directive::Directive(const String& dname,
					 const std::vector<String>& parameters,
					 Context context):
name(dname),
parameters(parameters),
enclosing(),
parent(),
enclosingContext(context)
{

}

void	Directive::addDirective(Directive* dir)
{
	directives.insert(std::make_pair(dir->name, dir));
}

const Directive*	Directive::getDirective(const String& key) const
{
	return directives.find(key)->second;
}

std::vector<Directive*>	Directive::getDirectives(const String& key) const
{
	typedef std::pair<String,Directive*>	keyValuePair;

	std::vector<Directive*>	matchingDirectives;
	Directive::EqualRange	range = directives.equal_range(key);
	while (range.first != range.second)
	{
		const keyValuePair&	entry = *range.first;
		Directive*			directive = entry.second;
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

Optional<String>
Directive::getParameterOf(const String& key) const
{
	typedef std::multimap<String,Directive*>::const_iterator	multimap_iterator;

	multimap_iterator	it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone<String>();
	}
	else
	{
		const std::vector<String>&	parameters = it->second->parameters;
		String						buffer;
		for (size_t i = 0; i < parameters.size(); ++i)
		{
			buffer += parameters[i];
			if (i != parameters.size() - 1)
			{
				buffer += ' ';
			}
		}
		return makeOptional(buffer);
	}
}

Optional< std::vector<String> >
Directive::getParametersOf(const String& key) const
{
	typedef std::multimap<String,Directive*>::const_iterator	multimap_iterator;

	multimap_iterator	it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone< std::vector<String> >();
	}
	else
	{
		return makeOptional(it->second->parameters);
	}
}

Optional< std::map<int,String> >	Directive::generateErrorPagesMapping() const
{
	std::map<int,String>		errorPages;
	const std::vector<String>&	errorPage = recursivelyLookup< std::vector<String> >("error_page")
										   .value_or(vector_of<String>());
	if (errorPage.size() < 2)
	{
		return makeNone< std::map<int,String> >();
	}
	const String&	page = errorPage.back();
	for (size_t i = 0; i < errorPage.size()-1; ++i)
	{
		std::pair<int,String> mapping = std::make_pair(errorPage[i].toInt(), page);
		errorPages.insert(mapping);
	}
	if (errorPages.empty())
	{
		return makeNone< std::map<int,String> >();
	}
	else
	{
		return makeOptional(errorPages);
	}
}

void	Directive::cleanUp()
{
	for (Mappings::iterator it = this->directives.begin();
		 it != this->directives.end();
		 ++it)
	{
		Directive*	child = it->second;
		child->cleanUp();
		delete child;
	}
}
