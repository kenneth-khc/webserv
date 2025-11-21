
#ifndef FILE_DESCRIPTOR_LIMIT
#define FILE_DESCRIPTOR_LIMIT

#include "String.hpp"
#include "NonFatal.hpp"

class	FileDescriptorLimit : public NonFatal
{
public:
	FileDescriptorLimit(const String& message);
};

#endif
