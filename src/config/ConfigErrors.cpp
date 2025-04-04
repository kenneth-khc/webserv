/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigErrors.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:57:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/04 22:45:01 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Context.hpp"
#include "ConfigErrors.hpp"
#include "Token.hpp"
#include "Logger.hpp"

/*****************
 * Config Errors *
 ****************/

const char*	ConfigError::what() const throw()
{
	return msg.c_str();
}

ConfigError::~ConfigError() throw() { }

/********************
 * Unexpected Token *
 *******************/

UnexpectedToken::UnexpectedToken(const Token& got,
								 const std::vector<Token>& expectedTokens)
{
	std::stringstream	errMsg;
	errMsg << "unexpected token `" << got.lexeme << "`, ";
	errMsg << "expected one of ";
	for (size_t i = 0; i < expectedTokens.size(); ++i)
	{
		errMsg << '`' << expectedTokens[i].lexeme << '`';
		if (i < expectedTokens.size()-1)
		{
			errMsg << " or ";
		}
	}
	Logger::error(errMsg);
	Logger::showErrorLine(got.diagnostic);
	msg = Logger::streamToString();
}

UnexpectedToken::UnexpectedToken(const Token& got, const Token& expected)
{
	Logger::error("unexpected token `" + got.lexeme + "`, "
				  "expected token `" + expected.lexeme + "`");
	Logger::showErrorLine(got.diagnostic);
	msg = Logger::streamToString();
}

UnexpectedToken::~UnexpectedToken() throw() { }

/*********************
 * Invalid Directive *
 ********************/

InvalidDirective::InvalidDirective(const Directive& directive)
{
	Logger::error("invalid directive `" + directive.name + "`");
	Logger::showErrorLine(directive.diagnostic);
	msg = Logger::streamToString();
}


InvalidDirective::InvalidDirective(const Directive* directive)
{
	Logger::error("invalid directive `" + directive->name + "`");
	Logger::showErrorLine(directive->diagnostic);
	msg = Logger::streamToString();
}

InvalidDirective::~InvalidDirective() throw() { }

/****************************
 * Invalid Parameter Amount *
 ***************************/

InvalidParameterAmount::InvalidParameterAmount(const Directive& directive,
											   size_t expectedAmount)
{
	std::stringstream	errMsg;
	errMsg << "invalid amount of parameters: got " << directive.parameters.size()
		   << ", expected " << expectedAmount
		   << " for directive `" << directive.name << '`';
	Logger::error(errMsg);
	Logger::showErrorLine(directive.diagnostic);
	msg = Logger::streamToString();
}

InvalidParameterAmount::InvalidParameterAmount(const Directive& directive,
											   size_t expectedMin,
											   size_t)
{
	std::stringstream	errMsg;
	errMsg << "invalid amount of parameters: got " << directive.parameters.size()
		   << ", expected at least " << expectedMin
		   << " for directive `" << directive.name << '`';
	Logger::error(errMsg);
	Logger::showErrorLine(directive.diagnostic);
	msg = Logger::streamToString();
}

InvalidParameterAmount::~InvalidParameterAmount() throw() { }

/*********************
 * Invalid Parameter *
 ********************/

InvalidParameter::InvalidParameter() { }

InvalidParameter::InvalidParameter(const Directive& directive, const Parameter& param)
{
	Logger::error("invalid parameter `" + param.value + "`, "
				  "at directive `" + directive.name + "`");
	Logger::showErrorLine(param.diagnostic);
	msg = Logger::streamToString();
}

InvalidParameter::~InvalidParameter() throw() { }

/*******************
 * Invalid Context *
 ******************/
InvalidContext::InvalidContext(const Directive& directive, Context )
{
	Logger::error("invalid context " + stringifyContext(directive.enclosingContext) +
				  " for parameter `" + directive.name + "`");
	if (directive.parent)
	{
		Logger::showErrorLine(directive.parent->diagnostic);
	}
	Logger::showErrorLine(directive.diagnostic);
	msg = Logger::streamToString();
}

InvalidContext::InvalidContext(const String& directive, Context context)
{
	std::stringstream	ss;

	ss << directive << " is in an invalid context " << stringifyContext(context);
	msg = ss.str();
}

InvalidContext::~InvalidContext() throw() { }

/***********************
 * Duplicate Directive *
 **********************/

DuplicateDirective::DuplicateDirective(const Directive& directive)
{
	std::stringstream	ss;

	ss << "Duplicate directive " << directive.name;
	if (directive.hasParameters())
	{
		ss << " ( = ";
		for (size_t i = 0; i < directive.parameters.size(); ++i)
		{
			ss << directive.parameters[i].value;
			if (i != directive.parameters.size()-1)
				ss << " ";
		}
		ss << ")";
	}
	ss << " found in " << stringifyContext(directive.enclosingContext);
	msg = ss.str();
}

DuplicateDirective::~DuplicateDirective() throw() { }

/*********************
 * Missing Directive *
 ********************/

MissingDirective::MissingDirective(const String& key)
{
	std::stringstream	ss;

	ss << "Missing directive " << key;
	msg = ss.str();
}

MissingDirective::~MissingDirective() throw() { }
