/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:23:35 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/04 20:49:00 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include "Configuration.hpp"
#include "Directive.hpp"
#include "ConfigErrors.hpp"

Configuration::Configuration() { }

void	Configuration::add(const Directive& dir)
{
	directives.insert(std::make_pair(dir.name, dir));
}

// TODO: refactor this
void	print(const Directive& dir, int indent = 0, int charStart = 0, bool lastRow = false)
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
	std::cout << dir.name;
	if (!dir.parameters.empty())
	{
		std::cout << " = ";
		for (size_t i = 0; i < dir.parameters.size(); ++i)
		{
			std::cout << dir.parameters[i];
			if (i < dir.parameters.size()-1)
				std::cout << " ";
		}
	}
		std::cout << '\n';
	const std::multimap<String,Directive>&	children = dir.directives;
	for (std::multimap<String,Directive>::const_iterator it = children.begin();
		 it != children.end(); ++it)
	{
		// WARN: I don't know where this charStart number came from
		if (it != --children.end())
			print(it->second, indent + 1, charStart + indent + 2, false);
		else
			print(it->second, indent + 1, charStart + indent + 2, true);
	}
}

Optional<Directive>	Configuration::find(const String& key) const
{
	std::multimap<String,Directive>::const_iterator	it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone<Directive>();
	}
	else
	{
		return makeOptional(it->second);
	}
}

void	Configuration::assertHasDirective(const String& key) const
{
	if (!find(key).exists)
	{
		throw MissingDirective(key);
	}
}

void	Configuration::display() const
{
	for (std::multimap<String,Directive>::const_iterator it = directives.begin();
		 it != directives.end(); ++it)
	{
		print(it->second, 0, 0, false);
	}
}

void	Configuration::printMatchingElements(const String& key)
{
	DirectiveRange	range = directives.equal_range(key);
	size_t			elements = std::distance(range.first, range.second);
	std::cout << "global" << " has " << elements
			  << " elements matching the key " << key << '\n';
	while (range.first != range.second)
	{
		const Directive&	dir = range.first->second;
		std::cout << dir.name;
		std::cout << " = ";
		if (dir.parameters.empty())
		{
			std::cout << "\"\"";
		}
		else
		{
			for (size_t i = 0; i < dir.parameters.size(); ++i)
			{
				std::cout << dir.parameters[i];
			}
		}
		std::cout << '\n';
		++range.first;
	}
}
