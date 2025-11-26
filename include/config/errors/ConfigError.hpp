#ifndef CONFIG_ERROR_HPP
#define CONFIG_ERROR_HPP

#include "String.hpp"
#include <stdexcept>

/* These are custom exceptions thrown by the Parser, to indicate things
 * going wrong when parsing a config file. It provides the ability to display
 * the error to the user, allowing them to correct the config file if they
 * so desire. */

class	ConfigError : std::runtime_error
{
public:
	ConfigError();
	ConfigError(const String& filename);
	~ConfigError() throw();

	virtual const char*	what() const throw() = 0;
	virtual String		format() const = 0;

protected:
	const String	filename;
};

#endif
