
#ifndef SETUP_ERROR_HPP
#define SETUP_ERROR_HPP

#include "String.hpp"

#include <stdexcept>

/** Exceptions thrown by the Driver for errors while setting up the
	servers */

class	SetupError : public std::runtime_error
{
public:
	SetupError(const String& message);
	~SetupError() throw();

	virtual const char*	what() const throw() = 0;
	virtual String		format() const = 0;

protected:
	String	message;
};

#endif

