
#include "SetupError.hpp"
#include "Fmt.hpp"
#include "String.hpp"
#include <stdexcept>

SetupError::SetupError(const String& message):
std::runtime_error(message),
message(message)
{
}

SetupError::~SetupError() throw()
{
}

const char*	SetupError::what() const throw()
{
	return "Setup error";
}

String	SetupError::format() const
{
	return Fmt().formatError(message);
}
