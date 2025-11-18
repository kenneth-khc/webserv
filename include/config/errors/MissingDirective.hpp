#ifndef MISSING_DIRECTIVE_HPP
#define MISSING_DIRECTIVE_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"
#include "String.hpp"

class MissingDirective : public ConfigError
{
public:
	MissingDirective(const Directive&, const String&);
	~MissingDirective() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive&	directive;
	const String		missingKey;
};

#endif
