#include "InvalidContext.hpp"
#include "ConfigError.hpp"
#include "Context.hpp"
#include "Formatter.hpp"
#include "Fmt.hpp"
#include "VectorInitializer.hpp"

InvalidContext::InvalidContext(const Directive& directive):
ConfigError(directive.diagnostic.filename),
directive(directive),
parent(directive.parent),
expectedContexts()
{
}

InvalidContext::InvalidContext(const Directive& directive, Context context):
ConfigError(directive.diagnostic.filename),
directive(directive),
parent(directive.parent),
expectedContexts()
{
	expectedContexts.push_back(context);
}

InvalidContext::InvalidContext(const Directive& directive,
							   const std::vector<Context>& contexts):
ConfigError(directive.diagnostic.filename),
directive(directive),
parent(directive.parent),
expectedContexts(contexts)
{
}

InvalidContext::~InvalidContext() throw() { }

const char*	InvalidContext::what() const throw()
{
	return "Invalid context";
}

String	InvalidContext::format() const
{
	std::stringstream	buf;
	std::stringstream	errMsg;
	std::stringstream	helpMsg;

	Fmt	fmt;
	if (parent == NULL)
	{
		fmt = Fmt(filename, vector_of(directive.diagnostic));
		errMsg << '`' << directive.name << "` not allowed globally";
		buf << fmt.formatError(errMsg.str())
			<< fmt.formatDiagnostic("declared globally here");
	}
	else
	{
		fmt = Fmt(filename, vector_of(parent->diagnostic)(directive.diagnostic));
		errMsg << '`' << directive.name << "` not allowed within `"  << parent->name << "` block";
		buf << fmt.formatError(errMsg.str())
			<< fmt.formatDiagnostic("block declared here")
			<< fmt.formatDiagnostic("for directive declared here");
	}
	if (expectedContexts.empty())
	{
	/* this triggers when a directive is placed under a block that is not
	   supposed to be a valid block and I cannot query what valid contexts
	   it should belong under */
		helpMsg << "remove `" << directive.name << "` from the `"
				<< parent->name << "` block";
	}
	else if (expectedContexts.size() == 1)
	{
		const Context&	ctx = expectedContexts.back();
		helpMsg << "place `" << directive.name << "` within "
				<< (ctx == GLOBAL ? "the " : "a ")
				<< stringifyContext(ctx) << " block";
	}
	else
	{
		helpMsg << "place `" << directive.name << "` within ";
		for (size_t i = 0; i < expectedContexts.size(); ++i)
		{
			helpMsg << '`' << stringifyContext(expectedContexts[i]) << '`';
			if (i == expectedContexts.size() - 2)
			{
				helpMsg << " or ";
			}
			else if (i != expectedContexts.size() - 1)
			{
				helpMsg << ", ";
			}
		}
		helpMsg << " block";
	}
	buf << fmt.formatHelp(helpMsg.str());
	return buf.str();
}

