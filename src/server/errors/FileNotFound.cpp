
#include "FileNotFound.hpp"
#include "Fmt.hpp"
#include "SetupError.hpp"

FileNotFound::FileNotFound(const String& message):
SetupError(message)
{
}

const char*	FileNotFound::what() const throw()
{
	return "File not found";
}

String	FileNotFound::format() const
{
	return Fmt().formatError(message + " (File not found)");
}
