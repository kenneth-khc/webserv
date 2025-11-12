#ifndef INVALID_CONTEXT_HPP
#define INVALID_CONTEXT_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"
#include "Context.hpp"

class	InvalidContext : public ConfigError
{
public:
	InvalidContext(const Directive&);
	InvalidContext(const Directive&, Context);
	InvalidContext(const Directive&, const std::vector<Context>&);
	~InvalidContext() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive&		directive;
	const Directive*		parent;
	std::vector<Context>	expectedContexts;
};

#endif
