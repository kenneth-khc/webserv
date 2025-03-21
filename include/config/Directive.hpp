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
#include <map>
#include "String.hpp"
#include "Context.hpp"

struct	Directive;

typedef std::multimap<String,Directive>	Mappings;

struct	Directive
{
	// Name of the current directive
	String				name;
	// Parameters of the current directive
	std::vector<String>	parameters;
	// The context this directive is in
	Context				enclosingContext;
	// The directives within the directive
	Mappings			directives;

	Directive();
	Directive(const String&, const std::vector<String>&, Context);

	void					addDirective(const Directive&);
	const Directive&		getDirective(const String&);
	std::vector<Directive>	getDirectives(const String& key) const;
	Optional<Directive>		find(const String& key) const;

	bool					hasParameters() const;
	template <typename ReturnType>
	Optional<ReturnType>	getParams(const String& key) const;
	void					printParameters() const;

};

typedef std::pair<std::multimap<String,Directive>::const_iterator,
				  std::multimap<String,Directive>::const_iterator> DirectiveRange;

template <>
inline Optional<String>
Directive::getParams<String>(const String& key) const
{
	std::multimap<String,Directive>::const_iterator it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone<String>();
	}
	else
	{
		const Directive&	dir = it->second;
		String				params;
		for (size_t i = 0; i < dir.parameters.size(); ++i)
		{
			params += dir.parameters[i];
			if (i != dir.parameters.size()-1)
			{
				params += ' ';
			}
		}
		return makeOptional(params);
	}
}

template <>
inline Optional< std::vector<String> >
Directive::getParams< std::vector<String> >(const String& key) const
{
	std::multimap<String,Directive>::const_iterator it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone< std::vector<String> >();
	}
	else
	{
		const Directive&	dir = it->second;
		return makeOptional(dir.parameters);
	}
}

#endif
