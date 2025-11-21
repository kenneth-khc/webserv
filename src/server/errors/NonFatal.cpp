
#include "NonFatal.hpp"
#include "String.hpp"

#include <stdexcept>

NonFatal::NonFatal(const String& message):
std::runtime_error(message)
{
}
