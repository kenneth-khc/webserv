/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diagnostic.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:57:27 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 19:59:45 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Diagnostic.hpp"

Diagnostic::Diagnostic():
	lineNum(0),
	columnNum(0)
{

}

Diagnostic::Diagnostic(std::size_t lineNum, std::size_t columnNum):
	lineNum(lineNum),
	columnNum(columnNum)
{

}
