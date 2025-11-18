#ifndef ADDRESS_NOT_AVAILABLE_HPP
#define ADDRESS_NOT_AVAILABLE_HPP

#include "SetupError.hpp"
#include "String.hpp"

class AddressNotAvailable : public SetupError
{
public:
	AddressNotAvailable(const String& msg);

	const char*	what() const throw();
	String		format() const;
};

#endif
