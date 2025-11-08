#include "Fmt.hpp"
#include "String.hpp"
#include "Colour.hpp"

using namespace Colour;

Fmt::Fmt(size_t gutterSize):
gutterSize(gutterSize)
{

}

String	Fmt::filename(const String& filename) const
{
	std::stringstream	buf;
		String				arrow;
		for (size_t i = 0; i < gutterSize; ++i)
		{
			arrow += '-';
		}
		arrow += "> ";
		buf << BOLD_BLUE << arrow << RESET
			<< filename << '\n';
	return buf.str();
}

String	Fmt::error(const String& msg) const
{
	std::stringstream	buf;
	buf << RED << "error: " << RESET;
	buf << BOLD_WHITE << msg << RESET << '\n';
	return buf.str();
}
