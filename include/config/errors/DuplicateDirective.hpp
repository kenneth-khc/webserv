#ifndef DUPLICATE_DIRECTIVE_HPP
#define DUPLICATE_DIRECTIVE_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"

class	DuplicateDirective : public ConfigError
{
public:
	DuplicateDirective(const Directive& prevDeclaration,
	                   const Directive& newDeclaration);
	~DuplicateDirective() throw();

	const char*	what() const throw();
	String		format() const;

	const Directive&	prevDeclaration;
	const Directive&	newDeclaration;
};

#endif
