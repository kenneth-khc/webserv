#include "InvalidParameter.hpp"
#include "Fmt.hpp"
#include <sstream>

InvalidParameter::InvalidParameter(const Directive& directive,
								   const Parameter& parameter):
directive(&directive),
diagnostic(parameter.diagnostic),
reason("")
{
}

InvalidParameter::InvalidParameter(const Directive& directive,
								   const Parameter& parameter,
								   const String& reason):
directive(&directive),
diagnostic(parameter.diagnostic),
reason(reason)
{
}

InvalidParameter::InvalidParameter(const Directive& directive,
								   const Parameter& parameter,
								   const String& reason,
								   const String& help):
directive(&directive),
diagnostic(parameter.diagnostic),
reason(reason),
help(help)
{
}

InvalidParameter::InvalidParameter(const Directive& directive,
								   const Diagnostic& diagnostic,
								   const String& reason,
								   const String& help):
directive(&directive),
diagnostic(diagnostic),
reason(reason),
help(help)
{
}

InvalidParameter::~InvalidParameter() throw() { }

const char*	InvalidParameter::what() const throw()
{
	return "Invalid parameter";
}

String	InvalidParameter::format() const
{
	std::stringstream	buf;
	std::stringstream	errmsg;
	Fmt					fmt = Fmt(diagnostic);

	errmsg << "invalid parameter at directive `" << directive->name << '`';
	buf << fmt.formatError(errmsg.str())
		<< fmt.formatDiagnostic(reason);
	if (!help.empty())
	{
		buf << fmt.formatHelp(help);
	}
	return buf.str();
}
