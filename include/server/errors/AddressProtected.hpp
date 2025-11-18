#ifndef ADDRESS_PROTECTED_HPP
#define ADDRESS_PROTECTED_HPP

#include "SetupError.hpp"
#include "String.hpp"

class AddressProtected: public SetupError
{
public:
	AddressProtected(const String& msg);

	const char*	what() const throw();
	String		format() const;
};

#endif
