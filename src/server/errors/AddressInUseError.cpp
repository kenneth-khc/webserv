#include "AddressInUseError.hpp"
#include "SetupError.hpp"
#include "Fmt.hpp"

AddressInUseError::AddressInUseError(const String& message):
SetupError(message)
{
}

const char*	AddressInUseError::what() const throw()
{
	return "Address in use";
}

String	AddressInUseError::format() const
{
	return Fmt().formatError(message + " (Address already in use)");
}
