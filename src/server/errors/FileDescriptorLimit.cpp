
#include "FileDescriptorLimit.hpp"
#include "NonFatal.hpp"
#include "String.hpp"

FileDescriptorLimit::FileDescriptorLimit(const String& message):
NonFatal(message)
{
}
