/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigErrors.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:57:59 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/21 18:43:51 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ConfigErrors.hpp"
#include "Token.hpp"

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

UnexpectedToken::UnexpectedToken(Token::TokenType got)
{
	std::stringstream	ss;

	ss << "Unexpeced token " << Token::stringified[got];
	msg = ss.str();
}

UnexpectedToken::UnexpectedToken(Token::TokenType expected, Token::TokenType got)
{
	std::stringstream	ss;

	ss << "Expected token " << Token::stringified[expected]
	   << ", got " << Token::stringified[got] << " instead...";
	msg = ss.str();
}

UnexpectedToken::~UnexpectedToken() throw() { }

/*********************
 * Invalid Directive *
 ********************/

InvalidDirective::InvalidDirective(const String& directive)
{
	std::stringstream	ss;
	
	ss << directive << " is an invalid directive you bozo";
	msg = ss.str();
}

InvalidDirective::~InvalidDirective() throw() { }

/****************************
 * Invalid Parameter Amount *
 ***************************/

InvalidParameterAmount::InvalidParameterAmount()
{
	std::stringstream	ss;
	
	ss << "invalid amount of parameters you bozo";
	msg = ss.str();
}

InvalidParameterAmount::~InvalidParameterAmount() throw() { }

/*********************
 * Invalid Parameter *
 ********************/

InvalidParameter::InvalidParameter() { }
InvalidParameter::InvalidParameter(const String& parameter)
{
	std::stringstream	ss;
	
	ss << parameter << " is an invalid parameter you bozo";
	msg = ss.str();
}

InvalidParameter::~InvalidParameter() throw() { }

/*******************
 * Invalid Context *
 ******************/

InvalidContext::InvalidContext(const String& directive, const String& context)
{
	std::stringstream	ss;

	ss << directive << " is in an invalid context " << context;
	msg = ss.str();
}

InvalidContext::~InvalidContext() throw() { }
