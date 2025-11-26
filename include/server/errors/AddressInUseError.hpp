
#ifndef ADDRESS_IN_USE_ERROR_HPP
#define ADDRESS_IN_USE_ERROR_HPP

#include "SetupError.hpp"
#include "String.hpp"

class AddressInUseError : public SetupError
{
public:
	AddressInUseError(const String& message);

	const char*	what() const throw(); // override
	String		format() const; // override
};

#endif
