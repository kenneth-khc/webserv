/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:57:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:00:38 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"
#include "Context.hpp"
#include <stdexcept>

Directive::Directive(const String& name,
					 const std::vector<Parameter>& parameters,
					 const Directive* parent,
					 const Diagnostic& diagnostic):
	name(name),
	parameters(parameters),
	parent(parent),
	/** we start with no children and the caller has to manually
		add them with .addDirective() */
	directives(),
	diagnostic(diagnostic)
{
}

Directive::~Directive()
{
	std::multimap<String, Directive*>::const_iterator iter;
	for (iter = this->directives.begin();
		 iter != this->directives.end();
		 ++iter)
	{
		Directive*	child = iter->second;
		delete child;
	}
}

void	Directive::addDirective(Directive* directive)
{
	std::pair<String, Directive*>	pair(directive->name, directive);

	directives.insert(pair);
}

const Directive&	Directive::get(const String& key) const
{
	std::multimap<String, Directive*>::const_iterator	iter =
 		directives.find(key);

	if (iter == directives.end())
	{
		throw std::out_of_range("directive " + key + " not found");
	}
	else
	{
		return *iter->second;
	}
}

Optional<Directive*>	Directive::getDirective(const String& key) const
{
	std::multimap<String, Directive*>::const_iterator	iter =
		directives.find(key);

	if (iter == directives.end())
	{
		return makeNone<Directive*>();
	}
	else
	{
		return makeOptional(iter->second);
	}
}

const std::multimap<String, Directive*>&
Directive::getDirectives() const
{
	return this->directives;
}

Diagnostic	Directive::getDiagnostic() const
{
	return this->diagnostic;
}

std::vector<Directive*>
Directive::getDirectives(const String& key) const
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

Context::Context	Directive::getContext() const
{
	if (this->parent == NULL)
	{
		return Context::GLOBAL;
	}
	else
	{
		return Context::from(this->parent->name);
	}
}

Parameter	Directive::getParameter() const
{
	String	buffer;
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		buffer += parameters[i].value;
		if (i != parameters.size() - 1)
		{
			buffer += ' ';
		}
	}
	return buffer;
}

const std::vector<Parameter>&	Directive::getParameters() const
{
	return this->parameters;
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
		const std::vector<Parameter>&	parameters = it->second->parameters;
		String						buffer;
		for (size_t i = 0; i < parameters.size(); ++i)
		{
			buffer += parameters[i].value;
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
		std::vector<String>	tmp;
		for(size_t i = 0; i < it->second->parameters.size(); ++i)
		{
			tmp.push_back(it->second->parameters[i].value);
		}
		return makeOptional(tmp);
	}
}

typedef std::map<int,String>	ErrorPageMapping;
Optional<ErrorPageMapping>	Directive::generateErrorPagesMapping() const
{
	typedef	std::vector<String>	StringVector;

	ErrorPageMapping	errorPages;
	const StringVector&	errorPage = recursivelyLookup<StringVector>("error_page")
								   .value_or(StringVector());
	if (errorPage.size() < 2)
	{
		return makeNone<ErrorPageMapping>();
	}
	const String&	page = errorPage.back();
	for (size_t i = 0; i < errorPage.size()-1; ++i)
	{
		std::pair<int,String> mapping = std::make_pair(errorPage[i].toInt(), page);
		errorPages.insert(mapping);
	}
	if (errorPages.empty())
	{
		return makeNone<ErrorPageMapping>();
	}
	else
	{
		return makeOptional(errorPages);
	}
}
