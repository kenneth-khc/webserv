
#ifndef NON_FATAL_HPP
#define NON_FATAL_HPP

#include "String.hpp"

#include <stdexcept>

class	NonFatal : public std::runtime_error
{
public:
	NonFatal(const String& message);
};

#endif
