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
#include <sstream>
#include <string>
#include "Optional.hpp"

/* Custom String object that wraps around std::string to provide us with 
 * facilities that help with parsing a std::string */

using std::string;

class	String
{
public:
	typedef string::size_type size_type;

	String();
	String(const char*);
	String(const std::string&);
	String(const String&);
	String&					operator=(const String&);
	char&					operator[](size_type);
	const char&				operator[](size_type) const;
	char&					operator*();
	char*					operator+(size_type);
	const char*				operator+(size_type) const;
	String					operator+(const String&);
	String&					operator+=(const String&);
	friend std::ostream&	operator<<(std::ostream&, const String&);

	char&				at(size_type);
	const char&			at(size_type) const;
	size_type			size() const;
	size_type			length() const;
	Optional<size_type>	find(const std::string&, size_type = 0);

	Optional<size_type>	findAfter(const std::string&, size_type = 0);
	bool				match(const String&, size_type = 0);
	bool				match(const std::string&, size_type = 0);

	bool		consume(const char*);
	bool		consume(const std::string&);
	bool		consume(const String&);

	bool		consumeIf(char c);
	bool		consumeUntil(const std::string&);

private:
	string	str;

	template <typename Type>
	static std::string	toStdString(const Type& T);
};


#endif
