/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:15:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/07 16:29:56 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <sstream>
#include <exception>
#include <vector>
#include <istream>
#include "Optional.hpp"

/* Custom String object that wraps around std::string to provide us with
 * facilities that help with parsing a std::string */

using std::string;
using std::vector;
using std::basic_istream;

struct	Predicate;

class	String
{
public:
	typedef string::size_type		size_type;
	typedef string::iterator		iterator;
	typedef string::const_iterator	const_iterator;

	const static size_type	npos = std::string::npos;

	/* Constructors */
	String();
	String(const char*);
	String(const std::string&);
	String(const String&);
	String(int);
	String(const std::ifstream&);

	/* Operators */
	bool					operator==(const String&) const;
	bool					operator!=(const String&) const;
	bool					operator<=(const String&) const;
	bool					operator>=(const String&) const;
	bool					operator<(const String&) const;
	bool					operator>(const String&) const;
	String&					operator=(const String&);
	char&					operator[](size_type);
	const char&				operator[](size_type) const;
	char&					operator*();
	char*					operator+(size_type);
	const char*				operator+(size_type) const;
	String					operator+(const String&);
	String					operator+(const String&) const;
	String					operator+(const char*);
	String					operator+(const char*) const;
	friend String			operator+(const char*, const String&);
	String&					operator+=(const String&);
	String&					operator+=(char);
	friend std::ostream&	operator<<(std::ostream&, const String&);
	operator	std::string() const;

	/* Wrapper to call std::string functions */

	char&				at(size_type);
	const char&			at(size_type) const;
	size_type			size() const;
	size_type			length() const;
	bool				empty() const;
	Optional<size_type>	find(char, size_type searchFrom = 0) const;
	Optional<size_type>	find(const String&, size_type searchFrom = 0) const;
	String				substr(size_type pos = 0, size_type len = npos) const;
	void				clear();
	const char*			c_str() const;
	Optional<size_type>	find_last_of(char, size_type searchFrom = npos) const;
	Optional<size_type>	find_last_of(const String&, size_type searchFrom = npos) const;
	void				resize(size_type);
	String&				replace(size_type, size_type, const String&);
	iterator			begin();
	const_iterator		begin() const;
	iterator			end();
	const_iterator		end() const;
	iterator			erase(iterator, iterator);

	/* Custom additions to a String */

	// Find the position after a given substring
	Optional<size_type>	findAfter(const String&, size_type searchFrom = 0) const;

	// Match a substr within a string
	bool				match(const String&, size_type matchFrom = 0);

	// Consume the first character in the string, returning it if successful
	Optional<char>		consume();

	// Consume a substring within string, returning it if successful
	Optional<String>	consume(const String&);

	// Consume the first character if it matches
	bool				consumeIf(char);

	// Consume the first character if Predicate returns true
	Optional<char>		consumeIf(const Predicate&);
	Optional<char>		consumeIf(bool (*pred)(char));

	// Consume characters up until a substring, returning the string consumed
	Optional<String>	consumeUntil(const String&);

	Optional<String>	consumeUntilNot(const Predicate&);

	// Consume characters up until a substring, assuming they all match the
	// Predicate, throwing on unexpected characters
	String				consumeIfUntil(const Predicate&, const String&);

	// Split the string into a vector<String> of each character
	vector<String>		split() const;

	// Split the string into a vector<String> based on the set of delimiters
	vector<String>		split(const String& delimiters) const;

	// Trim leading and trailing characters that match the set
	String				trim(const String& set);
	String				trim(const String& set) const;

	int					toInt() const;

	template <typename Type>
	static String	from(const Type& t)
	{
		std::stringstream	ss;
		ss << t;
		return String(ss.str());
	}
	// std::getline() "overload" for String class
	template<typename CharT, typename Traits>
	static basic_istream<CharT, Traits>&	getline(basic_istream<CharT, Traits>& input, String& str, CharT delim);
	template<typename CharT, typename Traits>
	static basic_istream<CharT, Traits>&	getline(basic_istream<CharT, Traits>& input, String& str);

private:
	string				str; // the underlying std::string

	// Helper to convert a Type into a std::string
	template <typename Type>
	static std::string	toStdString(const Type& T);
};

/* A functor for matching characters. By constructing a Predicate with a string,
 * we can then pass the functor into an operation which can then call
 * operator() on each character, matching the character by returning true/false */
struct	Predicate
{
public:
	Predicate(const std::string&);
	bool	operator()(const char&) const;

private:
	String	match;
};

// getline() overload template definitions
template<typename CharT, typename Traits>
basic_istream<CharT, Traits>&	String::getline(basic_istream<CharT, Traits>& input, String& str, CharT delim)
{
	std::string	stdString;

	std::getline(input, stdString, delim);
	str = stdString;
	return (input);
}

template<typename CharT, typename Traits>
basic_istream<CharT, Traits>&	String::getline(basic_istream<CharT, Traits>& input, String& str)
{
	std::string	stdString;

	std::getline(input, stdString);
	str = stdString;
	return (input);
}

// TODO: extend these with messages signalling what went wrong
class	ExpectedNotFound: public std::exception { };

class	UnexpectedCharacter: public std::exception { };

#endif
