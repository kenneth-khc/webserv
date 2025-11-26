#include "DuplicateDirective.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"
#include "VectorInitializer.hpp"

DuplicateDirective::DuplicateDirective(
const Directive& prevDeclaration,
const Directive& newDeclaration
):
// WARN: this assumes that both declarations belong within the same file
ConfigError(prevDeclaration.getDiagnostic().filename),
prevDeclaration(&prevDeclaration),
newDeclaration(&newDeclaration)
{
}

const char*	DuplicateDirective::what() const throw()
{
	return "Duplicate directive";
}

String	DuplicateDirective::format() const
{
	std::stringstream	buf;
	const String&		name = prevDeclaration->name;
	const Diagnostic&	prev = prevDeclaration->getDiagnostic();
	const Diagnostic&	curr = newDeclaration->getDiagnostic();
	Fmt					fmt = Fmt(filename, vector_of(prev)(curr));

	buf << fmt.formatError("duplicate directive `" + name + '`')
		<< fmt.formatDiagnostic("previously declared here")
		<< fmt.formatDiagnostic("redeclared here")
		<< fmt.formatHelp("remove a `" + name + "` declaration");
	return buf.str();
}

DuplicateDirective::~DuplicateDirective() throw() { }

