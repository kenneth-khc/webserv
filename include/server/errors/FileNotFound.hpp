
#ifndef FILE_NOT_FOUND_HPP
#define FILE_NOT_FOUND_HPP

#include "SetupError.hpp"
#include "String.hpp"

class FileNotFound : public SetupError
{
public:
	FileNotFound(const String& message);

	const char*	what() const throw(); // override
	String		format() const; // override
};

#endif
