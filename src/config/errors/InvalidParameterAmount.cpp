#include "InvalidParameterAmount.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"

InvalidParameterAmount::InvalidParameterAmount(const Directive& directive,
											   size_t expectedAmount):
ConfigError(directive.diagnostic.filename),
directive(directive),
expectedMin(expectedAmount),
expectedMax(expectedAmount)
{
}

InvalidParameterAmount::InvalidParameterAmount(const Directive& directive,
											   size_t expectedMin,
											   size_t expectedMax):
ConfigError(directive.diagnostic.filename),
directive(directive),
expectedMin(expectedMin),
expectedMax(expectedMax)
{
}

InvalidParameterAmount::~InvalidParameterAmount() throw() { }

const char*	InvalidParameterAmount::what() const throw()
{
	return "Invalid parameter amount";
}

String	InvalidParameterAmount::format() const
{
	std::stringstream				buf;
	std::stringstream				errmsg;
	const std::vector<Parameter>&	parameters = directive.parameters;
	const size_t					amount = parameters.size();

	if (amount > expectedMax)
	{
		if (expectedMin == expectedMax)
		{
			errmsg << "invalid amount of parameters: got " << amount << ", "
				   << "expected " << expectedMax << " for directive `"
				   << directive.name << '`';
		}
		else
		{
			errmsg << "invalid amount of parameters: got " << amount << ", "
				   << "expected at most " << expectedMax << " for directive `"
				   << directive.name << '`';
		}
		std::vector<Diagnostic>	extraParameters;
		for (size_t i = expectedMax; i < amount; ++i)
		{
			extraParameters.push_back(parameters[i].diagnostic);
		}

		Fmt	fmt = Fmt(filename, extraParameters);
		buf << fmt.formatError(errmsg.str());
		while (fmt.hasDiagnostics())
		{
			buf << fmt.formatDiagnostic();
		}
	}
	else
	{
		String		word;
		Diagnostic	missingParam;

		if (expectedMin == expectedMax)
		{
			errmsg << "invalid amount of parameters: got " << amount << ", "
				   << "expected " << expectedMax << " for directive `"
				   << directive.name << '`';
		}
		else
		{
			errmsg << "invalid amount of parameters: got " << amount << ", "
				   << "expected at least " << expectedMin << " for directive `"
				   << directive.name << '`';
		}
		if (parameters.size() == 0)
		{
			word = directive.name;
			missingParam = directive.diagnostic;
		}
		else
		{
			word = parameters.back().value;
			missingParam = parameters.back().diagnostic;
		}
		missingParam.columnNum += word.size() + 1;
		Fmt fmt = Fmt(filename, missingParam);
		buf << fmt.formatError(errmsg.str());
		buf << fmt.formatDiagnostic("expected parameter");
	}
	return buf.str();
}
