/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:15:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/06 19:08:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include "Optional.hpp"

/* Custom String object that wraps around std::string to provide us with 
 * facilities that help with parsing a std::string */

using std::string;
using std::vector;

struct	Predicate;

class	String
{
public:
	typedef string::size_type size_type;

	String();
	String(const char*);
	String(const std::string&);
	String(const String&);
	bool					operator==(const std::string&) const;
	bool					operator==(const String&) const;
	bool					operator==(const char*) const;
	bool					operator<(const String&) const;
	String&					operator=(const String&);
	char&					operator[](size_type);
	const char&				operator[](size_type) const;
	char&					operator*();
	char*					operator+(size_type);
	const char*				operator+(size_type) const;
	String					operator+(const String&);
	String&					operator+=(const String&);
	friend std::ostream&	operator<<(std::ostream&, const String&);
	operator	std::string() const;

	char&				at(size_type);
	const char&			at(size_type) const;
	size_type			size() const;
	size_type			length() const;
	Optional<size_type>	find(const char&, size_type = 0) const;
	Optional<size_type>	find(const std::string&, size_type = 0) const;
	Optional<size_type>	find(const String&, size_type = 0) const;
	String				substr(size_type pos = 0, size_type n = npos) const;

	Optional<size_type>	findAfter(const std::string&, size_type = 0) const;
	bool				match(const String&, size_type = 0);
	/*bool				match(const std::string&, size_type = 0);*/

	bool		consume();
	bool		consume(const char*);
	bool		consume(const std::string&);
	bool		consume(const String&);

	bool		consumeIf(char);
	bool		consumeIf(bool(*p)(char));
	bool		consumeIf(const Predicate&);
	String		consumeUntil(const std::string&);
	String		consumeUntil(const String&);
	bool		consumeIfUntil(const Predicate&, const std::string&);

	vector<String>	split() const;
	vector<String>	split(const std::string& delimiters) const;
	String			trim(const String& set);
	String			trim(const String& set) const;


private:
	string					str;
	const static size_type	npos = std::string::npos;

	template <typename Type>
	static std::string	toStdString(const Type& T);
};

struct	Predicate
{
public:
	Predicate(const std::string&);
	bool	operator()(const char&) const;

private:
	String	match;
};

class	ExpectedNotFound: public std::exception
{

};

class	UnexpectedCharacter: public std::exception
{

};

#endif
