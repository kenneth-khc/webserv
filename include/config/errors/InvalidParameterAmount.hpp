#ifndef INVALID_PARAMETER_AMOUNT_HPP
#define INVALID_PARAMETER_AMOUNT_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"

class	InvalidParameterAmount : public ConfigError
{
public:
	InvalidParameterAmount(const Directive&, size_t expectedAmount);
	InvalidParameterAmount(const Directive&, size_t min, size_t max);
	~InvalidParameterAmount() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive&	directive;
	const size_t		expectedMin;
	const size_t		expectedMax;
};

#endif
