#ifndef FMT_HPP
#define FMT_HPP

#include "String.hpp"
#include "Diagnostic.hpp"
#include <cstddef>
#include <vector>

class	Fmt
{
public:
	Fmt(const String& filename);
	Fmt(const Diagnostic&);
	Fmt(const String& filename, const std::vector<Diagnostic>&);

	String	formatFilename() const;
	String	formatError(const String& message) const;
	String	formatDiagnostic(const String& message);

private:
	size_t					gutterSize;
	String					filename;

	std::vector<Diagnostic>	diagnostics;
	size_t					diagnosticsDone;

	String	pad(const String& line, size_t columnNum) const;
	String	gutter(size_t size) const;
	String	gutter(size_t size, size_t lineNum) const;

	String	lineInfo(size_t lineNum, size_t colNum) const;
	String	diagnosis(const String& msg, size_t lineNum, size_t colNum) const;
	String	gutterLine() const;
};

#endif
