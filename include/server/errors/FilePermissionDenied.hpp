
#ifndef FILE_PERMISSION_DENIED_HPP
#define FILE_PERMISSION_DENIED_HPP

#include "SetupError.hpp"
#include "String.hpp"

class FilePermissionDenied : public SetupError
{
public:
	FilePermissionDenied(const String& message);

	const char*	what() const throw(); // override
	String		format() const; // override
};

#endif
