#include "AddressProtected.hpp"
#include "Fmt.hpp"

AddressProtected::AddressProtected(const String& message):
SetupError(message)
{
}

const char*	AddressProtected::what() const throw()
{
	return "Address protected";
}

String	AddressProtected::format() const
{
	return Fmt().formatError(message +
							" (Insufficient permissions, address is protected)");
}
