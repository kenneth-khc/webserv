#ifndef INVALID_CONTEXT_HPP
#define INVALID_CONTEXT_HPP

#include "ConfigError.hpp"
#include "Context.hpp"
#include "Directive.hpp"

class	InvalidContext : public ConfigError
{
public:
	InvalidContext(const Directive&);
	InvalidContext(const Directive&, Context::Context);
	InvalidContext(const Directive&, const std::vector<Context::Context>&);
	~InvalidContext() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive&				directive;
	const Directive*				parent;
	std::vector<Context::Context>	expectedContexts;
};

#endif
