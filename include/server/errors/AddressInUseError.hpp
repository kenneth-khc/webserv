
#ifndef ADDRESS_IN_USE_ERROR_HPP
#define ADDRESS_IN_USE_ERROR_HPP

#include "SetupError.hpp"
#include "String.hpp"

class AddressInUseError : public SetupError
{
public:
	AddressInUseError(const String& msg);

	const char*	what() const throw();
	String		format() const;
};

#endif
