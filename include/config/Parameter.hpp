/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parameter.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:24:32 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:04:10 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "String.hpp"
#include "Diagnostic.hpp"

class	Parameter
{
public:
	String		value;
	Diagnostic	diagnostic;

	Parameter(const char*);
	Parameter(const String&);
	Parameter(const String&, const Diagnostic&);

	bool		operator==(const Parameter&) const;
	bool		operator<(const Parameter&) const;
	char&		operator[](size_t index);
	const char&	operator[](size_t index) const;

	operator	String();
	operator	const String() const;

private:
	Parameter	operator=(const Parameter&);
};

#endif
