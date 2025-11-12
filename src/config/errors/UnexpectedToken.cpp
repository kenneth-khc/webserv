#include "UnexpectedToken.hpp"
#include "ConfigError.hpp"
#include "Fmt.hpp"
#include "Token.hpp"
#include "VectorInitializer.hpp"

UnexpectedToken::UnexpectedToken(const Token& currToken,
								 const Token& prevToken,
								 const Token& expected):
ConfigError(currToken.diagnostic.filename),
currToken(currToken),
prevToken(prevToken),
expected()
{
	this->expected.push_back(expected);
}

UnexpectedToken::UnexpectedToken(const Token& currToken,
								 const Token& prevToken,
								 const std::vector<Token>& expected):
ConfigError(currToken.diagnostic.filename),
currToken(currToken),
prevToken(prevToken),
expected(expected)
{
	
}

UnexpectedToken::~UnexpectedToken() throw() { }

const char*	UnexpectedToken::what() const throw()
{
	return "Unexpected token";
}

String	UnexpectedToken::format() const
{
	std::stringstream	buf;
	std::stringstream	errMsg;
	std::stringstream	helpMsg;

	if (currToken.type == Token::END_OF_FILE)
	{
		Fmt	fmt = Fmt(filename, vector_of(prevToken.diagnostic)(currToken.diagnostic));

		errMsg << "unexpected end of file";
		buf << fmt.formatError(errMsg.str())
			<< fmt.formatDiagnostic("for token here")
			<< fmt.formatDiagnostic("unexpected eof here");
		if (prevToken.type == Token::LCURLY)
		{
			buf << fmt.formatHelp("close the block with a '}'");
		}
		else if (prevToken.type == Token::NAME)
		{
			buf << fmt.formatHelp("terminate the directive with a `;`");
		}
	}
	else
	{
		Fmt	fmt = Fmt(filename, currToken.diagnostic);

		errMsg << "unexpected token `" << currToken.lexeme << '`';
		if (expected.size() == 1 && expected[0].type == Token::NAME)
		{
			if (currToken.type == Token::LCURLY)
			{
				helpMsg << "declare a directive before starting a new block";
			}
			else if (currToken.type == Token::SEMICOLON)
			{
				helpMsg << "declare a directive before terminating with `;`";
			}
			else if (currToken.type == Token::RCURLY)
			{
				helpMsg << "declare a new block before terminating with `}`";
			}
		}
		else if (expected.size() > 1)
		{
			if (prevToken.type == Token::NAME && currToken.type == Token::RCURLY)
			{
				helpMsg << "start a new block with `{` or terminate the directive with `;`";
			}
			else
			{
				helpMsg << "expected one of " << Fmt::formatList(expected);
			}
		}
		buf << fmt.formatError(errMsg.str())
			<< fmt.formatDiagnostic("unexpected");
		if (helpMsg.rdbuf()->in_avail())
		{
			buf << fmt.formatHelp(helpMsg.str());
		}
	}
	return buf.str();
}

