/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:28:11 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/18 16:31:50 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <fstream>
#include <stdexcept>
#include "String.hpp"
#include "Optional.hpp"

String::String():
str("") { }

String::String(const char* cStr):
str(cStr) { }

String::String(const std::string& str):
str(str) { }

String::String(const String& other):
str(other.str) { }

String::String(int num)
{
	std::stringstream	ss;
	ss << num;
	str = ss.str();
}

String::String(const std::ifstream& ifs)
{
	std::stringstream	ss;
	ss << ifs.rdbuf();
	str = ss.str();
}

bool	String::operator==(const String& rhs) const
{
	return str == rhs.str;
}

bool	String::operator!=(const String& rhs) const
{
	return str != rhs.str;
}

bool	String::operator<=(const String& rhs) const
{
	return str <= rhs.str;
}

bool	String::operator>=(const String& rhs) const
{
	return str >= rhs.str;
}

bool	String::operator<(const String& rhs) const
{
	return str < rhs.str;
}

bool	String::operator>(const String& rhs) const
{
	return str > rhs.str;
}

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

String	String::operator+(const String& rhs)
{
	return String(str + rhs.str);
}

String	String::operator+(const String& rhs) const
{
	return String(str + rhs.str);
}

String	String::operator+(const char* rhs)
{
	return *this + String(rhs);
}

String	String::operator+(const char* rhs) const
{
	return *this + String(rhs);
}

String	operator+(const char* lhs, const String& rhs)
{
	return String(lhs) + rhs;
}

String&	String::operator+=(const String& rhs)
{
	str += rhs.str;
	return *this;
}

String&	String::operator+=(char rhs)
{
	str += rhs;
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

bool	String::empty() const
{
	return str.empty();
}

String::operator std::string() const
{
	return str;
}

Optional<String::size_type>
String::find(char c, size_type offset) const
{
	String::size_type	pos = str.find(c, offset);
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
String::find(const String& expected, size_type offset) const
{
	size_type	pos = str.find(expected, offset);
	if (pos == str.npos)
	{
		return makeNone<size_type>();
	}
	else
	{
		return makeOptional(pos);
	}
}

String	String::substr(size_type pos, size_type n) const
{
	return str.substr(pos, n);
}

void	String::clear()
{
	return str.clear();
}

const char*	String::c_str() const
{
	return str.c_str();
}

Optional<String::size_type>
String::find_last_of(char c, size_type offset) const
{
	size_type	pos = str.find_last_of(c, offset);
	if (pos == str.npos)
	{
		return makeNone<size_type>();
	}
	else
	{
		return makeOptional<size_type>(pos);
	}
}

Optional<String::size_type>
String::find_last_of(const String& expected, size_type offset) const
{
	size_type	pos = str.find_last_of(expected, offset);
	if (pos == str.npos)
	{
		return makeNone<size_type>();
	}
	else
	{
		return makeOptional<size_type>(pos);
	}
}

void	String::resize(size_type count)
{
	str.resize(count);
}

String&	String::replace(size_type pos, size_type count, const String& toReplace)
{
	str.replace(pos, count, toReplace);
	return *this;
}

String&	String::replace(String::iterator first, String::iterator last, const String& toReplace)
{
	str.replace(first, last, toReplace);
	return *this;
}

String::iterator	String::begin()
{
	return str.begin();
}

String::const_iterator	String::begin() const
{
	return str.begin();
}

String::iterator	String::end()
{
	return str.end();
}

String::const_iterator	String::end() const
{
	return str.end();
}

String::reverse_iterator	String::rbegin()
{
	return str.rbegin();
}

String::const_reverse_iterator	String::rbegin() const
{
	return str.rbegin();
}

String::reverse_iterator	String::rend()
{
	return str.rend();
}

String::const_reverse_iterator	String::rend() const
{
	return str.rend();
}

String&	String::erase(String::size_type index, String::size_type count)
{
	str.erase(index, count);
	return *this;
}

String::iterator	String::erase(String::iterator first, String::iterator last)
{
	return str.erase(first, last);
}

Optional<String::size_type>
String::findAfter(const String& expected, size_type offset) const
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
}

Optional<char>	String::consume()
{
	if (empty())
	{
		return makeNone<char>();
	}
	else
	{
		char	consumed = str[0];
		str.erase(0, 1);
		return makeOptional(consumed);
	}
}

Optional<String>	String::consume(const String& expected)
{
	if (match(expected))
	{
		String	consuming = str.substr(0, expected.length());
		str = str.substr(expected.length());
		return makeOptional(consuming);
	}
	else
	{
		return makeNone<String>();
	}
}

Predicate::Predicate(const std::string& str):
match(str) { }

bool	Predicate::operator()(const char& c) const
{
	if (match.find(c).exists)
	{
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
		consume();
		return true;
	}
	return false;
}

Optional<char>	String::consumeIf(const Predicate& pred)
{
	if (!str.empty() && pred(str[0]))
	{
		char	consumed = consume().value;
		return makeOptional(consumed);
	}
	return makeNone<char>();
}

Optional<char>	String::consumeIf(bool (*pred)(char))
{
	if (!str.empty() && pred(str[0]))
	{
		char	consumed = consume().value;
		return makeOptional(consumed);
	}
	return makeNone<char>();
}

Optional<String>	String::consumeUntil(const String& expected)
{
	Optional<size_type>	pos = find(expected);
	if (pos.exists)
	{
		String	consumed = str.substr(0, pos.value);
		str = str.substr(pos.value);
		return consumed;
	}
	else
	{
		return makeNone<String>();
	}
}

Optional<String>	String::consumeUntilNot(const Predicate& pred)
{
	String			consumedString;
	Optional<char>	consumed = consumeIf(pred);
	while (consumed.exists)
	{
		consumedString += consumed.value;
		consumed = consumeIf(pred);
	}
	if (consumedString.empty())
	{
		return makeNone<String>();
	}
	else
	{
		return makeOptional(consumedString);
	}
}

String	String::consumeIfUntil(const Predicate& pred, const String& expected)
{
	Optional<size_type>	pos = find(expected);
	if (!pos.exists)
	{
		throw ExpectedNotFound();
	}
	else
	{
		String	consumedString;
		for (size_t i = 0; i < pos.value; ++i)
		{
			Optional<char>	consumed = consumeIf(pred);
			if (consumed.exists)
			{
				consumedString += consumed.value;
			}
			else
			{
				throw UnexpectedCharacter();
			}
		}
		return consumedString;
	}
}

vector<String>	String::split() const
{
	vector<String>	strings;
	std::string		s = str;

	for (std::string::iterator it = s.begin(); it != s.end(); ++it)
	{
		String	singleCharToken = std::string(1, *it);
		strings.push_back(singleCharToken);
	}
	return strings;
}

vector<String>	String::split(const String& delimiters) const
{
	vector<String>	strings;
	std::string		s = str;

	while (s.length() != 0)
	{
		size_type	delimiterPos = s.find_first_of(delimiters);
		String		token = s.substr(0, delimiterPos);
		if (token.length() > 0)
		{
			strings.push_back(token);
		}
		s.erase(0, token.length()+1);
	}
	return strings;
}

String	String::trim(const String& set)
{
	if (str.empty())
	{
		return str;
	}
	size_type	start = 0;
	while (set.find(str[start]).exists)
	{
		++start;
	}
	size_type	end = str.size()-1;
	while (set.find(str[end]).exists)
	{
		--end;
	}
	size_type	len = (end + 1) - start;
	str = str.substr(start, len);
	return str;
}

String	String::trim(const String& set) const
{
	if (str.empty())
	{
		return str;
	}
	size_type	start = 0;
	while (set.find(str[start]).exists)
	{
		++start;
	}
	size_type	end = str.size()-1;
	while (set.find(str[end]).exists)
	{
		--end;
	}
	size_type	len = (end + 1) - start;
	return str.substr(start, len);
}

int	String::toInt() const
{
	int	num;
	std::istringstream(str) >> num;
	return num;
}

bool	String::starts_with(const String& prefix) const
{
	if (prefix.str.length() > str.length())
	{
		return false;
	}

	String::const_iterator	it = str.begin();
	String::const_iterator	prefixIt = prefix.str.begin();
	while (prefixIt != prefix.str.end())
	{
		if (*it != *prefixIt)
		{
			return false;
		}
		it++;
		prefixIt++;
	}
	return true;
}

bool	String::ends_with(const String& suffix) const
{
	if (suffix.str.length() > str.length())
	{
		return false;
	}

	String::const_reverse_iterator	it = str.rbegin();
	String::const_reverse_iterator	suffixIt = suffix.str.rbegin();
	while (suffixIt != suffix.str.rend())
	{
		if (*it != *suffixIt)
		{
			return false;
		}
		it++;
		suffixIt++;
	}
	return true;
}

String	String::lower() const
{
	String	lowercase;

	for (String::size_type i = 0; i < str.length(); i++)
	{
		lowercase += std::tolower(str[i]);
	}
	return lowercase;
}

String	String::upper() const
{
	String	uppercase;

	for (String::size_type i = 0; i < str.length(); i++)
	{
		uppercase += std::toupper(str[i]);
	}
	return uppercase;
}

String	String::title() const
{
	String	titlecase;
	bool	toUpper = true;

	for (String::size_type i = 0; i < str.length(); i++)
	{
		if (toUpper && std::isalpha(str[i]) != 0)
		{
			titlecase += std::toupper(str[i]);
			toUpper = false;
		}
		else
		{
			titlecase += std::tolower(str[i]);
		}
		if (std::isalpha(str[i]) == 0)
		{
			toUpper = true;
		}
	}
	return titlecase;
}

template <typename Type>
std::string	String::toStdString(const Type& T)
{
	std::stringstream	ss;
	ss << T;
	return ss.str();
}
