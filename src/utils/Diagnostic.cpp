/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diagnostic.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:57:27 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/04 16:29:44 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Diagnostic.hpp"

Diagnostic::Diagnostic():
filename(""),
lineNum(1),
colNum(1)
{
}

Diagnostic::Diagnostic(const String& filename, size_t lineNum, size_t colNum):
filename(filename),
lineNum(lineNum),
colNum(colNum)
{
}

Diagnostic::Diagnostic(const Diagnostic& other):
filename(other.filename),
lineNum(other.lineNum),
colNum(other.colNum)
{
}

Diagnostic	Diagnostic::operator=(const Diagnostic& other)
{
	if (&other != this)
	{
		this->filename = other.filename;
		this->lineNum = other.lineNum;
		this->colNum= other.colNum;
	}
	return *this;
}
