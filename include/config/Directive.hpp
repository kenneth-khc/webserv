/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:32:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/16 01:30:21 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <vector>
#include <map>
#include "String.hpp"
#include "Context.hpp"

struct	Directive;

typedef std::multimap<String,Directive*>	Mappings;

struct	Directive
{
	// Name of the current directive
	String				name;

	// Parameters of the current directive
	std::vector<String>	parameters;

	// The enclosing block
	Mappings			enclosing;

	//
	const Directive*	parent;

	// The context this directive is in
	Context				enclosingContext;

	// The directives within the directive
	std::multimap<String,Directive*>	directives;

	Directive();
	Directive(const String&, const std::vector<String>&, Context);

	void					addDirective(const Directive&);
	void					addDirective(Directive* dir);
	const Directive*		getDirective(const String&);
	std::vector<Directive*>	getDirectives(const String& key) const;
	Optional<String>		recursivelyLookup(const String&) const;
	Optional<Directive*>	find(const String& key) const;

	bool					hasParameters() const;
	template <typename ReturnType>
	Optional<ReturnType>	getParams(const String& key) const;
	void					printParameters() const;

	void					cleanUp();

private:
	// Functor to pass into optional.or_else() to recursively lookup a value
	// if not found in current scope
	struct	LookupEnclosing
	{
		const Directive*	directive;
		const String&		key;

		LookupEnclosing(const Directive* directive, const String& key):
			directive(directive),
			key(key) {}

		Optional<String>	operator()() const
		{
			const Directive*	enclosing = directive->parent;
			if (enclosing == NULL)
			{
				return makeNone<String>();
			}
			else
			{
				return enclosing->recursivelyLookup(key);
			}
		}
	};
	/*Optional<String>	lookupEnclosing(const String&) const;*/
};

typedef std::pair<std::multimap<String,Directive*>::const_iterator,
				  std::multimap<String,Directive*>::const_iterator> DirectiveRange;

template <>
inline Optional<String>
Directive::getParams<String>(const String& key) const
{
	std::multimap<String,Directive*>::const_iterator it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone<String>();
	}
	else
	{
		const Directive&	dir = *it->second;
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
	std::multimap<String,Directive*>::const_iterator it = directives.find(key);
	if (it == directives.end())
	{
		return makeNone< std::vector<String> >();
	}
	else
	{
		const Directive&	dir = *it->second;
		return makeOptional(dir.parameters);
	}
}

#endif
