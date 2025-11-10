#include "Fmt.hpp"
#include "String.hpp"
#include "Colour.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <stdexcept>

using namespace Colour;

bool	cmp(const Diagnostic& lhs, const Diagnostic& rhs)
{
	if (lhs.lineNum > rhs.lineNum)
	{
		return true;
	}
	return false;
}

Fmt::Fmt(const String& filename):
filename(filename),
diagnostics(),
diagnosticsDone(0)
{
	gutterSize = 5;
}

Fmt::Fmt(const Diagnostic& diagnostic):
filename(diagnostic.filename),
diagnosticsDone(0)
{
	diagnostics.push_back(diagnostic);
	gutterSize = countDigits(diagnostic.lineNum) + 2;
}

Fmt::Fmt(const String& filename, const std::vector<Diagnostic>& diagnostics):
filename(filename),
diagnostics(diagnostics),
diagnosticsDone(0)
{
	const std::vector<Diagnostic>::const_iterator iter =
		std::max_element(diagnostics.begin(), diagnostics.end(), cmp);
	gutterSize = countDigits(iter->lineNum) + 2;
}

String	Fmt::formatFilename() const
{
	std::stringstream	buf;
	String				arrow;

	for (size_t i = 0; i < gutterSize; ++i)
	{
		arrow += '-';
	}
	arrow += "> ";
	buf << BOLD_BLUE << arrow << RESET
		<< filename << '\n';

	return buf.str();
}

String	Fmt::formatError(const String& msg) const
{
	std::stringstream	buf;
	buf << RED << "error: " << RESET;
	buf << BOLD_WHITE << msg << RESET << '\n';
	return buf.str();
}

String	Fmt::lineInfo(size_t lineNum, size_t colNum) const
{
	std::stringstream	buf;
	String				arrow;
	for (size_t i = 0; i < gutterSize; ++i)
	{
		arrow += '-';
	}
	arrow += "> ";
	buf << BOLD_BLUE << arrow << RESET
		<< filename << ':' << lineNum << ':' << colNum << '\n';
	return buf.str();
}

String	Fmt::formatDiagnostic(const String& message)
{
	if (diagnosticsDone >= diagnostics.size())
	{
		throw std::out_of_range("No more diagnostics left to be formatted");
	}

	std::stringstream	buf;
	const Diagnostic&	diagnostic = diagnostics[diagnosticsDone];
	const size_t		lineNum = diagnostic.lineNum;
	const size_t		colNum = diagnostic.columnNum;

	buf << lineInfo(lineNum, colNum)
		<< gutterLine()
		<< diagnosis(message, lineNum, colNum);
	++diagnosticsDone;

	return buf.str();
}

// TODO(kecheong): refactor this
String	Fmt::diagnosis(const String& message, size_t lineNum, size_t colNum) const
{
	std::stringstream	buf;
	const String&		line = getLineFromFile(filename, lineNum);

	buf << gutter(gutterSize, lineNum) << line << '\n'
		<< gutter(gutterSize) << pad(line, colNum)
		<< BOLD_RED << "^ " << message << RESET;
	
	return buf.str();
}

String	Fmt::pad(const String& line, size_t columnNum) const
{
	String	padding;
	for (size_t i = 0; i < columnNum-1; ++i)
	{
		if (line[i] == '\t')
		{
			padding += '\t';
		}
		else
		{
			padding += ' ';
		}
	}
	return padding;
}

String	Fmt::gutter(size_t size) const
{
	std::stringstream	buf;
	String				gutter;

	for (std::size_t i = 0; i < size; ++i)
	{
		gutter += ' ';
	}
	buf << BOLD_BLUE << gutter << "| " << RESET;
	return buf.str();
}

String	Fmt::gutter(size_t size, size_t lineNumLabel) const
{
	std::stringstream	buf;
	String				gutter;
	std::size_t			digits = countDigits(lineNumLabel);

	for (std::size_t i = 0; i < size - digits - 1; ++i)
	{
		gutter += ' ';
	}
	buf << BOLD_BLUE << gutter << lineNumLabel << " | " << RESET;
	return buf.str();
}

String	Fmt::gutterLine() const
{
	std::stringstream	buf;

	for (size_t i = 0; i < gutterSize - 1; ++i)
	{
		buf << ' ';
	}
	buf << BOLD_BLUE << " | " << RESET << '\n';
	return buf.str();
}
