#include "MissingDirective.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"

MissingDirective::MissingDirective(const String& filename, const String& key):
ConfigError(filename),
directiveKey(key)
{

}

MissingDirective::~MissingDirective() throw()
{

}

const char*	MissingDirective::what() const throw()
{
	return "Missing directive";
}

String	MissingDirective::format() const
{
	Fmt					fmt(1);
	std::stringstream	buf;

	buf << fmt.filename(filename)
		<< fmt.error("missing directive `" + directiveKey + '`');
	return buf.str();
}
