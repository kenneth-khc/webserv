/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:32:54 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/28 21:54:08 by kecheong         ###   ########.fr       */
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

	String		normalize(const String&) const;
	String		resolve(const String&, const String&) const;
	String		resolveWithPrefix(const String&) const;

private:
	String	prefix;

	bool		isAbsolute(const String&) const;
	bool		isRelative(const String&) const;
};

#endif
