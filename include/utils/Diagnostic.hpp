/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diagnostic.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:56:10 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 20:36:35 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAGNOSTIC_HPP
#define DIAGNOSTIC_HPP

/* An object providing information for diagnostics when something goes wrong.
 *
 * Encoded into a Token, Directive and its Parameters, so that when a
 * configuration option goes wrong we can immediately get the line and column
 * number and print around the error. */

#include "String.hpp"
#include <cstddef>

struct	Diagnostic
{
	/* NOTE: cannot be const as the Lexer holds a copy of the current token
	         so this has to be copied over each time */
	String	filename;
	size_t	lineNum;
	size_t	colNum;

	Diagnostic();
	Diagnostic(const Diagnostic&);
	Diagnostic(const String& filename, size_t lineNum, size_t colNum);

	Diagnostic	operator=(const Diagnostic&);
};

#endif
