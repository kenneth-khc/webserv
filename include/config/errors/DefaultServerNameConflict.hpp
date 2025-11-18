#ifndef DEFAULT_SERVER_NAME_CONFLICT_HPP
#define DEFAULT_SERVER_NAME_CONFLICT_HPP

#include "ConfigError.hpp"
#include "Directive.hpp"

class DefaultServerNameConflict : public ConfigError
{
public:
	DefaultServerNameConflict(const Directive& server1,
							  const Directive& server2,
							  const String& address);
	~DefaultServerNameConflict() throw();

	const char*	what() const throw();
	String		format() const;

private:
	const Directive&	server1;
	const Directive&	server2;
	const String		address;
};

#endif
