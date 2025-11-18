
#include "AddressNotAvailable.hpp"
#include "Fmt.hpp"

AddressNotAvailable::AddressNotAvailable(const String& message):
SetupError(message)
{
}

const char*	AddressNotAvailable::what() const throw()
{
	return "Address not available";
}

String	AddressNotAvailable::format() const
{
	return Fmt().formatError(message + " (Address not available)");
}
