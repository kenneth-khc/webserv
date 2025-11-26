/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:32:54 by kecheong          #+#    #+#             */
/*   Updated: 2025/11/21 08:23:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATHHANDLER_HPP
#define PATHHANDLER_HPP

#include "String.hpp"

class	PathHandler
{
public:
	PathHandler();
	PathHandler(const String&);
	void		setPrefix(const String&);

	String		resolve(const String&, const String&) const;
	String		resolveWithPrefix(const String&) const;

private:
	String	prefix;

	bool		isAbsolute(const String&) const;
	bool		isRelative(const String&) const;
};

#endif
