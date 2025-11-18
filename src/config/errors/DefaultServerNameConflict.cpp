#include "DefaultServerNameConflict.hpp"
#include "Directive.hpp"
#include "Fmt.hpp"
#include "VectorInitializer.hpp"

DefaultServerNameConflict::DefaultServerNameConflict(const Directive& server1,
													 const Directive& server2,
													 const String& address):
server1(server1),
server2(server2),
address(address)
{
}

DefaultServerNameConflict::~DefaultServerNameConflict() throw()
{
}

const char*	DefaultServerNameConflict::what() const throw()
{
	return "Default server name conflict";
}

String		DefaultServerNameConflict::format() const
{
	std::stringstream	buf;
	Fmt	fmt = Fmt(vector_of(server1.getDiagnostic())(server2.getDiagnostic()));

	buf << fmt.formatError("multiple default server_names on address " + address)
		<< fmt.formatDiagnostic("this server has no server_name")
		<< fmt.formatDiagnostic("this server also has no server_name")
		<< fmt.formatHelp("each address can only have one default server");
	return buf.str();
}
