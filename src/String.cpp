/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:28:11 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 23:46:23 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "String.hpp"
#include <stdexcept>

template <typename Type>
std::string	String::toStdString(const Type& T)
{
	std::stringstream	ss;
	ss << T;
	return ss.str();
}

String::String():
str("") { }

String::String(const char* cStr):
str(cStr) { }

String::String(const std::string& str):
str(str) { }

String::String(const String& other):
str(other.str) { }

String&	String::operator=(const String& other)
{
	str = other.str;
	return *this;
}

char&	String::operator[](size_type index)
{
	return str[index];
}

const char&	String::operator[](size_type index) const
{
	return str[index];
}

char&	String::operator*()
{
	return str[0];
}

char*	String::operator+(size_type offset)
{
	return &str[offset];
}

const char*	String::operator+(size_type offset) const
{
	return &str[offset];
}

String	String::operator+(const String& other)
{
	return String(str + other.str);
}

String&	String::operator+=(const String& other)
{
	str += other.str;
	return *this;
}

std::ostream&	operator<<(std::ostream& os, const String& s)
{
	os << s.str;
	return os;
}

char&	String::at(size_type index)
{
	if (index >= str.length())
	{
		std::string	wantIndex = toStdString(index);
		std::string	size = toStdString(str.length());
		std::string	message = "String::at(n) which is " + wantIndex +
							  " >= this->length() which is " + size + '\n';
		throw std::out_of_range(message);
	}
	return str[index];
}

const char&	String::at(size_type index) const
{
	if (index >= str.length())
	{
		std::string	wantIndex = toStdString(index);
		std::string	size = toStdString(str.length());
		std::string	message = "String::at(n) which is " + wantIndex +
							  " >= this->length() which is " + size + '\n';
		throw std::out_of_range(message);
	}
	return str[index];
}

String::size_type	String::size() const
{
	return str.size();
}

String::size_type	String::length() const
{
	return str.length();
}

Optional<String::size_type>
String::find(const std::string& expected, size_type offset)
{
	String::size_type	pos = str.find(expected, offset);
	if (pos == str.npos)
	{
		return makeNone<size_type>();
	}
	else
	{
		return makeOptional(pos);
	}
}

Optional<String::size_type>
String::findAfter(const std::string& expected, size_type offset)
{
	Optional<size_type>	pos = find(expected, offset);
	if (pos.exists)
	{
		pos.value += expected.length();
	}
	return pos;
}

bool	String::match(const String& expected, size_type offset)
{
	return str.compare(offset, expected.length(), expected.str) == 0;
	/*for (size_t i = 0; i < expected.size(); ++i, ++offset)*/
	/*{*/
	/*	if (str[offset] != expected[i])*/
	/*	{*/
	/*		return false;*/
	/*	}*/
	/*}*/
	/*return true;*/
}

bool	String::match(const std::string& expected, size_type offset)
{
	return str.compare(offset, expected.length(), expected) == 0;
	/*for (size_t i = 0; i < expected.size(); ++i, ++offset)*/
	/*{*/
	/*	if (str[offset] != expected[i])*/
	/*	{*/
	/*		return false;*/
	/*	}*/
	/*}*/
	/*return true;*/
}

bool	String::consume(const char* cStr)
{
	std::string	expected(cStr);

	if (match(expected))
	{
		str = str.substr(expected.length());
		return true;
	}
	else
	{
		return false;
	}
}

bool	String::consume(const std::string& expected)
{
	if (match(expected))
	{
		str = str.substr(expected.length());
		return true;
	}
	else
	{
		return false;
	}
}

bool	String::consume(const String& expected)
{
	if (match(expected))
	{
		str = str.substr(expected.length());
		return true;
	}
	else
	{
		return false;
	}
}

bool	String::consumeIf(char c)
{
	if (!str.empty() && str[0] == c)
	{
		str.erase(0, 1);
		return true;
	}
	return false;
}

bool	String::consumeUntil(const std::string& expected)
{
	Optional<size_type>	pos = find(expected);
	if (pos.exists)
	{
		str = str.substr(pos.value);
		return true;
	}
	else
	{
		return false;
	}
}
