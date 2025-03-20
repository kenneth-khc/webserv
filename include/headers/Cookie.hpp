/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:41:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:06:11 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
# define COOKIE_HPP

# include <vector>
# include <map>
# include "String.hpp"

class Response;

class Cookie {
	public:
		String						name;
		String						value;
		Optional<String>			expires;
		Optional<String::size_type>	maxAge;
		Optional<String>			domain;
		Optional<String>			path;
		bool						secure;
		bool						httpOnly;

		Cookie(void);
		Cookie(const String &name, const String &value);
		~Cookie(void);
		Cookie(const Cookie &obj);
		Cookie	&operator=(const Cookie &obj);

		void	operator=(const String &value);
		bool	operator==(const Cookie &obj) const;
		String	constructSetCookieHeader(void) const;
};

bool	isCookieString(std::map<String, Cookie> &cookies, const String &line);
bool	isCookiePair(const String &line);
bool	isCookieValue(const String &line);
bool	isCookieOctet(const unsigned &character);

#endif
