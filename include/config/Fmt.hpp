#ifndef FMT_HPP
#define FMT_HPP

#include "String.hpp"
#include "Diagnostic.hpp"
#include "Token.hpp"
#include "Context.hpp"
#include <cstddef>
#include <vector>

class	Fmt
{
public:
	Fmt();
	Fmt(const String& filename);
	Fmt(const Diagnostic&);
	Fmt(const std::vector<Diagnostic>&);
	Fmt(const String& filename, const Diagnostic&);
	Fmt(const String& filename, const std::vector<Diagnostic>&);

	bool	hasDiagnostics() const;
	String	formatFilename() const;
	String	formatError(const String& message) const;
	String	formatDiagnostic();
	String	formatDiagnostic(const String& message);
	String	formatHelp(const String& helpMessage) const;

	static String	formatList(const std::vector<Token>& tokens);
	static String	formatList(const std::vector<Context::Context>& contexts);

private:
	size_t					gutterSize;
	String					filename;

	std::vector<Diagnostic>	diagnostics;
	size_t					diagnosticsDone;

	String	getLineFromFile(const String& filename, size_t lineNum) const;
	int		countDigits(size_t) const;

	String	pad(const String& line, size_t columnNum) const;
	String	pad(const String& line, size_t startColNum, size_t endColNum) const;
	String	gutter(size_t size) const;
	String	gutter(size_t size, size_t lineNum) const;

	String	lineInfo(size_t lineNum, size_t colNum) const;
	String	diagnosis(const String& msg, size_t lineNum, size_t colNum) const;
	String	gutterLine() const;

};

#endif
