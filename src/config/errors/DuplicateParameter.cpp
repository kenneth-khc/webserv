
#include "DuplicateParameter.hpp"
#include "Fmt.hpp"
#include "Parameter.hpp"
#include "VectorInitializer.hpp"


DuplicateParameter::DuplicateParameter(const Directive& directive,
									   const Parameter& prev,
									   const Parameter& curr):
directive(directive),
prev(prev),
curr(curr)
{
}

DuplicateParameter::~DuplicateParameter() throw() { }

const char*	DuplicateParameter::what() const throw()
{
	return "Duplicate parameter";
}

String	DuplicateParameter::format() const
{
	std::stringstream	buf;
	Fmt	fmt = Fmt(vector_of(prev.diagnostic)(curr.diagnostic));

	buf << fmt.formatError("duplicate parameter within `" + directive.name + "`")
		<< fmt.formatDiagnostic("previously declared here")
		<< fmt.formatDiagnostic("redeclared here")
		<< fmt.formatHelp("a file extension can only be assigned to one cgi script handler");

	return buf.str();
}
