#include "MissingGlobalDirective.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"

MissingGlobalDirective::MissingGlobalDirective(const String& filename, const String& key):
ConfigError(filename),
directiveKey(key)
{

}

MissingGlobalDirective::~MissingGlobalDirective() throw()
{

}

const char*	MissingGlobalDirective::what() const throw()
{
	return "Missing directive";
}

String	MissingGlobalDirective::format() const
{
	Fmt					fmt = Fmt(filename);
	std::stringstream	buf;

	buf << fmt.formatFilename()
		<< fmt.formatError("missing directive `" + directiveKey + '`');
	return buf.str();
}
