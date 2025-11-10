#include "DuplicateDirective.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"
#include "VectorInitializer.hpp"

DuplicateDirective::DuplicateDirective(
const Directive& prevDeclaration,
const Directive& newDeclaration
):
// WARN: this assumes that both declarations belong within the same file
ConfigError(prevDeclaration.diagnostic.filename),
prevDeclaration(prevDeclaration),
newDeclaration(newDeclaration)
{
}

const char*	DuplicateDirective::what() const throw()
{
	return "Duplicate directive";
}

String	DuplicateDirective::format() const
{
	std::stringstream	buf;
	const Diagnostic&	prev = prevDeclaration.diagnostic;
	const Diagnostic&	curr = newDeclaration.diagnostic;
	Fmt					fmt = Fmt(filename, vector_of(prev)(curr));

	buf << fmt.formatDiagnostic("previously declared here")
		<< fmt.formatDiagnostic("redeclared here");
	return buf.str();
}

DuplicateDirective::~DuplicateDirective() throw() { }

