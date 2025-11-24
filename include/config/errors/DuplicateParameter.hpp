#ifndef DUPLICATE_PARAMETER_HPP
#define DUPLICATE_PARAMETER_HPP

#include "ConfigError.hpp"
#include "Parameter.hpp"

class	DuplicateParameter : public ConfigError
{
public:
	// DuplicateParameter(const Directive& prevDeclaration,
	//                    const Directive& newDeclaration);
	DuplicateParameter(const Parameter& prev, const Parameter& curr);
	~DuplicateParameter() throw();

	const char*	what() const throw();
	String		format() const;

	// const Directive&	prevDeclaration;
	// const Directive&	newDeclaration;

	Parameter	prev;
	Parameter	curr;
};

#endif
