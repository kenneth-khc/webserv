#include "ServerNameConflict.hpp"
#include "Directive.hpp"
#include "Fmt.hpp"
#include "VectorInitializer.hpp"

#include <sstream>

ServerNameConflict::ServerNameConflict(const Directive& server1,
									   const Directive& server2,
									   const String& address,
									   const String& serverName):
server1(server1),
server2(server2),
address(address),
serverName(serverName)
{
}

ServerNameConflict::~ServerNameConflict() throw()
{
}

const char*	ServerNameConflict::what() const throw()
{
	return "Server name conflict";
}

String	ServerNameConflict::format() const
{
	std::stringstream	buf;
	std::stringstream	errMsg;
	Fmt	fmt = Fmt(vector_of(server1.getDiagnostic())(server2.getDiagnostic()));

	errMsg << "conflicting server_name `" << serverName
		   << "` on address " << address;
	buf << fmt.formatError(errMsg.str())
		<< fmt.formatDiagnostic("server name `" + serverName + "` within here")
		<< fmt.formatDiagnostic("server name `" + serverName + "` also within here")
		<< fmt.formatHelp("each address cannot share the same server_name");
	return buf.str();
}
