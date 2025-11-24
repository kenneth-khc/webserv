
#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

#include "String.hpp"

#include <vector>

/**
	These are default values for each directive, collected here as a single
	source of truth. Any initialization that requires a default value should
	simply call Defaults::KEY, rather than hard coding the value in the source
	code which would be much less maintainable.
*/

class Defaults
{
public:
	static const String					TYPES;
	static const std::vector<String>	SERVER_NAME;
	static const String					LISTEN;
	static const String					ROOT;
	static const String					ALIAS;
	static const std::vector<String>	INDEX;
	static const String					AUTOINDEX;
	static const String					CLIENT_MAX_BODY_SIZE;
	static const std::vector<String>	ALLOW_METHODS;
	static const String					UPLOAD_DIRECTORY;
};

#endif
