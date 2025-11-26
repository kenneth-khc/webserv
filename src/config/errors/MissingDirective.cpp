#include "MissingDirective.hpp"
#include "ConfigError.hpp"
#include "Directive.hpp"
#include "Fmt.hpp"
#include "String.hpp"

MissingDirective::MissingDirective(const Directive& directive,
                                   const String& missingKey):
ConfigError(directive.getDiagnostic().filename),
directive(&directive),
missingKey(missingKey)
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
	std::stringstream	buf;
	Fmt	fmt = Fmt(directive->getDiagnostic());

	buf << fmt.formatError("missing `" + missingKey + "` in `" +
	                       directive->name + "` block")
		<< fmt.formatDiagnostic("block declared here")
		<< fmt.formatHelp("add a " + missingKey + " directive within");
	return buf.str();
}
