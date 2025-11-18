#include "InvalidDirective.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"

InvalidDirective::InvalidDirective(const Directive& directive):
ConfigError(directive.getDiagnostic().filename),
directive(directive)
{
}


InvalidDirective::InvalidDirective(const Directive* directive):
ConfigError(directive->getDiagnostic().filename),
directive(*directive)
{
}

InvalidDirective::~InvalidDirective() throw() { }

const char*	InvalidDirective::what() const throw()
{
	return "Invalid directive";
}

String	InvalidDirective::format() const
{
	std::stringstream	buf;
	Fmt					fmt = Fmt(directive.getDiagnostic());

	buf << fmt.formatError("invalid directive `" + directive.name + '`')
		<< fmt.formatDiagnostic("declared here");
	return buf.str();
}
