#include "ConfigError.hpp"
#include <stdexcept>

ConfigError::ConfigError():
std::runtime_error("Config error")
{

}

ConfigError::~ConfigError() throw() { }

ConfigError::ConfigError(const String& filename):
std::runtime_error("Config error"),
filename(filename)
{

}

