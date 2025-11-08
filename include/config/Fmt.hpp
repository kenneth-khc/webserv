#ifndef FMT_HPP
#define FMT_HPP

#include "String.hpp"
#include <cstddef>

class	Fmt
{
public:
	Fmt(size_t gutterSize);

	String	filename(const String&) const;
	String	error(const String& msg) const;

private:
	size_t	gutterSize;
};

#endif
