#ifndef UNEXPECTED_TOKEN_HPP
#define UNEXPECTED_TOKEN_HPP

#include "ConfigError.hpp"
#include "Token.hpp"

class	UnexpectedToken : public ConfigError
{
public:
	UnexpectedToken(const Token& currToken,
					const Token& prevToken,
					const Token& expected);

	UnexpectedToken(const Token& currToken,
					const Token& prevToken,
					const std::vector<Token>& expected);

	~UnexpectedToken() throw();

	const char*	what() const throw();
	String		format() const;

private:
	Token				currToken;
	Token				prevToken;
	std::vector<Token>	expected;
};


#endif

