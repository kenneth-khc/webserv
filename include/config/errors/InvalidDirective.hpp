#ifndef INVALID_DIRECTIVE_HPP
#define INVALID_DIRECTIVE_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"

class	InvalidDirective : public ConfigError
{
public:
	InvalidDirective(const Directive&);
	InvalidDirective(const Directive*);
	~InvalidDirective() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive	directive;
};

#endif
