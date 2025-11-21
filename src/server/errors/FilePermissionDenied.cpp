
#include "FilePermissionDenied.hpp"
#include "Fmt.hpp"
#include "SetupError.hpp"
#include "String.hpp"

FilePermissionDenied::FilePermissionDenied(const String& message):
SetupError(message)
{
}

const char*	FilePermissionDenied::what() const throw()
{
	return "File permission denied";
}

String	FilePermissionDenied::format() const
{
	return Fmt().formatError(message + " (Permission denied)");
}
