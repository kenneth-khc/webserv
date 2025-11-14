/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:23:35 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/05 22:15:00 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"
#include "Directive.hpp"
#include "Optional.hpp"
#include <iostream>
#include <stdexcept>

Configuration::Configuration():
directives()
{
}

Configuration::~Configuration()
{
	for (Mappings::iterator it = directives.begin();
		 it != directives.end();
		 ++it)
	{
		Directive*	directive = it->second;
		directive->cleanUp();
		delete directive;
	}
}

void	Configuration::add(Directive* dir)
{
	directives.insert(std::make_pair(dir->name, dir));
}

const Directive&	Configuration::get(const String& key) const
{
	DirectiveMappingsConstIter	iter = directives.find(key);
	if (iter == directives.end())
	{
		throw std::out_of_range("directive " + key + " not found");
	}
	else
	{
		return *iter->second;
	}
}

Optional<Directive*>	Configuration::getDirective(const String& key) const
{
	DirectiveMappingsConstIter	iter = directives.find(key);
	if (iter == directives.end())
	{
		return makeNone<Directive*>();
	}
	else
	{
		return makeOptional(iter->second);
	}
}

void	Configuration::display() const
{
	for (DirectiveMappingsConstIter iter = directives.begin();
		 iter != directives.end();
		 ++iter)
	{
		print(*iter->second, 0, 0, false);
	}
	std::cout << "--------------------------------\n";
}

void	Configuration::print(const Directive& directive,
							 int indent,
							 int charStart,
							 bool lastRow) const
{
	for (int i = 0; i < charStart; ++i)
	{
		std::cout << " ";
	}
	if (charStart != 0)
	{
		if (!lastRow)
			std::cout << "├";
		else
			std::cout << "└";
	}
	for (int i = 0; i < indent; ++i)
	{
		std::cout << "─";
	}
	if (charStart != 0)
		std::cout << " ";
	std::cout << directive.name;
	if (!directive.parameters.empty())
	{
		std::cout << " = ";
		for (size_t i = 0; i < directive.parameters.size(); ++i)
		{
			std::cout << directive.parameters[i].value;
			if (i < directive.parameters.size()-1)
				std::cout << " ";
		}
	}
		std::cout << '\n';
	const std::multimap<String,Directive*>&	children = directive.directives;
	for (std::multimap<String,Directive*>::const_iterator it = children.begin();
		 it != children.end(); ++it)
	{
		// WARN: I don't know where this charStart number came from
		if (it != --children.end())
			print(*it->second, indent + 1, charStart + indent + 2, false);
		else
			print(*it->second, indent + 1, charStart + indent + 2, true);
	}
}
