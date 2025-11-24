
#include "DuplicateParameter.hpp"
#include "Fmt.hpp"
#include "Parameter.hpp"


DuplicateParameter::DuplicateParameter(const Parameter& prev, const Parameter& curr):
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
	Fmt	fmt = Fmt();

	buf << fmt.formatError("duplicate parameter `" + curr.value + "`");

	return buf.str();
}
