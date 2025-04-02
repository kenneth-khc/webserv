/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:32:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 20:59:27 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "String.hpp"
#include "Context.hpp"
#include "Optional.hpp"
#include "Parameter.hpp"
#include "Diagnostic.hpp"
#include <vector>
#include <map>

struct	Directive;

typedef std::multimap<String,Directive*>	Mappings;

struct	Directive
{
	Directive();
	Directive(const String&, const std::vector<String>&, Context);
	Directive(const String&, const std::vector<Parameter>&, Context);

/* A range of all Directives with the same matching key in a multimap */
typedef std::pair<std::multimap<String,Directive*>::const_iterator,
				  std::multimap<String,Directive*>::const_iterator> EqualRange;

	// Name of the current directive
	String				name;

	// Parameters of the current directive
	/*std::vector<String>	parameters;*/
	std::vector<Parameter>	parameters;

	// The enclosing block
	Mappings			enclosing;

	//
	const Directive*	parent;

	// The context this directive is in
	Context				enclosingContext;

	// The directives within the directive
	std::multimap<String,Directive*>	directives;

	void					addDirective(Directive*);

	const Directive*		getDirective(const String&) const;
	std::vector<Directive*>	getDirectives(const String& key) const;

	bool					hasParameters() const;

	Optional<String>
	getParameterOf(const String& key) const;

	Optional< std::vector<String> >
	getParametersOf(const String& key) const;

	template <typename ReturnType>
	Optional<ReturnType>	recursivelyLookup(const String&) const;

	void					printParameters() const;
	Optional< std::map<int,String> >	generateErrorPagesMapping() const;

	void					cleanUp();

	Diagnostic	diagnostic;

private:
	// Functor to pass into optional.or_else() to recursively lookup a value
	// if not found in current scope
	template <typename ReturnType>
	struct	LookupEnclosing
	{
		const Directive*	directive;
		const String&		key;

		LookupEnclosing(const Directive*, const String&);
		Optional<ReturnType>	operator()() const;

	private:
		LookupEnclosing();
		LookupEnclosing(const LookupEnclosing&);
		LookupEnclosing&	operator=(const LookupEnclosing&);
	};
};

template <typename ReturnType>
Directive::LookupEnclosing<ReturnType>
::LookupEnclosing(const Directive* directive, const String& key):
	directive(directive),
	key(key) {}

template <typename ReturnType>
Optional<ReturnType>
Directive::LookupEnclosing<ReturnType>::operator()() const
{
	const Directive*	enclosing = directive->parent;
	if (enclosing == NULL)
	{
		return makeNone<ReturnType>();
	}
	else
	{
		return enclosing->recursivelyLookup<ReturnType>(key);
	}
}

template <>
inline Optional<String>
Directive::recursivelyLookup<String>(const String& key) const
{
	return getParameterOf(key)
		  .or_else(LookupEnclosing<String>(this, key));
}

template <>
inline Optional< std::vector<String> >
Directive::recursivelyLookup< std::vector<String> >(const String& key) const
{
	return getParametersOf(key)
		  .or_else(LookupEnclosing< std::vector<String> >(this, key));
}

#endif
