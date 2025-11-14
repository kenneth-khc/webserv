#include "InvalidContext.hpp"
#include "ConfigError.hpp"
#include "Context.hpp"
#include "Fmt.hpp"
#include "VectorInitializer.hpp"

InvalidContext::InvalidContext(const Directive& directive):
ConfigError(directive.getDiagnostic().filename),
directive(directive),
parent(directive.parent),
expectedContexts()
{
}

InvalidContext::InvalidContext(const Directive& directive, Context::Context context):
ConfigError(directive.getDiagnostic().filename),
directive(directive),
parent(directive.parent),
expectedContexts()
{
	expectedContexts.push_back(context);
}

InvalidContext::InvalidContext(const Directive& directive,
							   const std::vector<Context::Context>& contexts):
ConfigError(directive.getDiagnostic().filename),
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
		fmt = Fmt(filename, vector_of(directive.getDiagnostic()));
		errMsg << '`' << directive.name << "` not allowed globally";
		buf << fmt.formatError(errMsg.str())
			<< fmt.formatDiagnostic("declared globally here");
	}
	else
	{
		fmt = Fmt(filename, vector_of(parent->getDiagnostic())
									 (directive.getDiagnostic()));
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
		const Context::Context&	ctx = expectedContexts.back();
		helpMsg << "place `" << directive.name << "` within "
				<< (ctx == Context::GLOBAL ? "the " : "a ")
				<< Context::toString(ctx) << " block";
	}
	else
	{
		helpMsg << "place `" << directive.name << "` within ";
		helpMsg << Fmt::formatList(expectedContexts) << " block";
	}
	buf << fmt.formatHelp(helpMsg.str());
	return buf.str();
}

