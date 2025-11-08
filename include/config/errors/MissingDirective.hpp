#ifndef MISSING_DIRECTIVE_HPP
#define MISSING_DIRECTIVE_HPP

#include "ConfigError.hpp"

class	MissingDirective : public ConfigError
{
public:
	MissingDirective(const String& filename, const String& directiveName);
	~MissingDirective() throw();

	const char*	what() const throw();
	String		format() const;

private:
	String	directiveKey;
};

#endif
