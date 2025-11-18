
#ifndef SERVER_NAME_CONFLICT_HPP
#define SERVER_NAME_CONFLICT_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"

class ServerNameConflict : public ConfigError
{
public:
	ServerNameConflict(const Directive& server1,
					   const Directive& server2,
					   const String& address,
					   const String& serverName);
	~ServerNameConflict() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive&	server1;
	const Directive&	server2;
	const String		address;
	const String		serverName;
};

#endif
