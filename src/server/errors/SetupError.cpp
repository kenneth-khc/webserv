
#include "SetupError.hpp"
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
