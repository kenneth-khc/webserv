/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:32:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/04 23:39:20 by kecheong         ###   ########.fr       */
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

struct	Directive
{
public:

/** A key-value mapping of DirectiveName -> Directive */
typedef std::multimap<String,Directive*>	Map;

/** Iterator for traversing the Name->Directive mappings */
typedef Map::iterator	MapIter;

/** Const iterator for traversing the Name->Directive mappings */
typedef Map::const_iterator	MapConstIter;

/** A range of all Directives with the same matching key in a multimap */
typedef std::pair<MapConstIter, MapConstIter>	EqualRange;

	/** Constructs a Directive with the given name, parameters, parent
		and diagnostic information.
		The Directive is not guaranteed to be a valid one until it gets
		passed into a Validator to be checked. */
	Directive(const String& name,
			  const std::vector<Parameter>& parameters,
			  const Directive* parent,
			  const Diagnostic&);

	/** Add a child Directive within this Directive block */
	void
	addDirective(Directive*);

	/** Returns a Directive matching the given key, throws an
		exception if the Directive does not exist */
	const Directive&		get(const String&) const;

	/** Returns a Directive matching the given key, wrapped in an
		Optional */
	Optional<Directive*>	getDirective(const String& key) const;

	/** Returns a vector of all Directives matching the given key */
	std::vector<Directive*>
	getDirectives(const String& key) const;

	/** Returns all the Directives within this block */
	const std::multimap<String, Directive*>&
	getDirectives() const;

	/** Returns the Parameter(s) for this Directive, reduced into a
		single String */
	Parameter	getParameter() const;

	/** Returns the vector of Parameters for this Directive */
	const std::vector<Parameter>&	getParameters() const;

	/** Returns the Parameter of a Directive within this block
		reduced into a single String,
		or None if the Directive is not found */
	Optional<String>
	getParameterOf(const String& key) const;

	/** Returns all the Parameters of a Directive within this block,
		or None if the Directive is not found */
	Optional< std::vector<String> >
	getParametersOf(const String& key) const;


	/** Looks up a the Parameters of a Directive within this block.
		If not found, recursively look up from the enclosing blocks,
		until it is found or there are no enclosing blocks left.
		This is how we "inherit" Directives from outer blocks.

		Returns None if the Directive cannot be found.  */
	template <typename ReturnType>
	Optional<ReturnType>
	recursivelyLookup(const String&) const;

	Optional< std::map<int,String> >
	generateErrorPagesMapping() const;

	/** Get the context of this Directive by transforming
		the parent's name into a Context */
	Context::Context	getContext() const;

	/** Get the diagnostic information for this Directive */
	const Diagnostic&	getDiagnostic() const;

	/** Cleans up the children Directives before cleaning up this Directive */
	void	cleanUp();

	/** Name of the current Directive */
	String	name;

	/** Parameters of the current Directive */
	std::vector<Parameter>	parameters;

	/** The parent Directive block enclosing this Directive,
		or NULL if this Directive is declared globally */
	const Directive*	parent;


private:
	/** Not expected to construct a bare Directive */
	Directive();
	/** Not expected to copy a Directive */
	Directive(const Directive&);

	/** The child directives within this block */
	std::multimap<String,Directive*>	directives;

	/** Diagnostic information for this Directive */
	Diagnostic	diagnostic;

	/** Functor to pass into optional.or_else() to recursively lookup a value
		if not found in the current scope */
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

/** Creates a functor storing the current Directive and the key to lookup,
	to be invoked by .or_else() when the Directive is not found within
	the current scope */
template <typename ReturnType>
Directive::LookupEnclosing<ReturnType>
::LookupEnclosing(const Directive* directive, const String& key):
	directive(directive),
	key(key) {}

/** Get the enclosing block and look up the key within it.
	If there is no enclosing block we are in the outermost block
	and the Directive cannot be found, returning None */
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

/** Recursively lookup the Parameters of a Directive, specialized
	to return the found Parameters as a single String */
template <>
inline Optional<String>
Directive::recursivelyLookup<String>(const String& key) const
{
	return getParameterOf(key)
		  .or_else(LookupEnclosing<String>(this, key));
}

/** Recursively lookup the Parameters of a Directive, specialized
	to return the Parameters as a vector of Strings*/
template <>
inline Optional< std::vector<String> >
Directive::recursivelyLookup< std::vector<String> >(const String& key) const
{
	return getParametersOf(key)
		  .or_else(LookupEnclosing< std::vector<String> >(this, key));
}

#endif
