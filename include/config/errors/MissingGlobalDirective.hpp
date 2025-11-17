#ifndef MISSING_GLOBAL_DIRECTIVE_HPP
#define MISSING_GLOBAL_DIRECTIVE_HPP

#include "ConfigError.hpp"

class	MissingGlobalDirective : public ConfigError
{
public:
	MissingGlobalDirective(const String& filename, const String& directiveName);
	~MissingGlobalDirective() throw();

	const char*	what() const throw();
	String		format() const;

private:
	String	directiveKey;
};

#endif
