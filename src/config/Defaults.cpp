
#include "Defaults.hpp"
#include "VectorInitializer.hpp"

#define BYTE "1"
#define KILOBYTE "1000"
#define MEGABYTE "1000000"

const String				Defaults::TYPES = "mime.types";
const std::vector<String>	Defaults::SERVER_NAME = std::vector<String>();
const String				Defaults::LISTEN = "0.0.0.0:8000";
const String				Defaults::ROOT = "html";
const std::vector<String>	Defaults::INDEX = vector_of<String>("index.html");
const String				Defaults::AUTOINDEX = "off";
const String				Defaults::CLIENT_MAX_BODY_SIZE = MEGABYTE;
const std::vector<String>	Defaults::ALLOW_METHODS = vector_of<String>("GET");
const String				Defaults::UPLOAD_DIRECTORY = String();
