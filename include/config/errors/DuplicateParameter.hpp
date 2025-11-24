#ifndef DUPLICATE_PARAMETER_HPP
#define DUPLICATE_PARAMETER_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"
#include "Parameter.hpp"

class	DuplicateParameter : public ConfigError
{
public:
	DuplicateParameter(const Directive& directive,
	                   const Parameter& prev,
	                   const Parameter& curr);
	~DuplicateParameter() throw();

	const char*	what() const throw();
	String		format() const;

	Directive	directive;
	Parameter	prev;
	Parameter	curr;
};

#endif
