#ifndef INVALID_PARAMETER_HPP
#define INVALID_PARAMETER_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"

class	InvalidParameter : public ConfigError
{
public:
	/* creates an InvalidParameter with no reason given */
	InvalidParameter(const Directive&, const Parameter&);

	/* creates an InvalidParameter with a reason to be displayed */
	InvalidParameter(const Directive&, const Parameter&, const String&);

	/* creates an InvalidParameter with a reason and help to be displayed */
	InvalidParameter(const Directive&, const Parameter&, const String&, const String&);

	InvalidParameter(const Directive&, const Diagnostic&, const String&);
	InvalidParameter(const Directive&, const Diagnostic&, const String&, const String&);

	~InvalidParameter() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive		directive;
	const Diagnostic	diagnostic;
	const String		reason;
	const String		help;
};

#endif
