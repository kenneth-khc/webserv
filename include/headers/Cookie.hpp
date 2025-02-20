/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:41:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/19 23:52:55 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
# define COOKIE_HPP

# include <vector>
# include <map>
# include "String.hpp"

# define NUM_OF_NAMES 1

class Response;

class Cookie {
	public:
		String		name;
		String		value;
		String		expires;
		std::size_t	maxAge;
		String		domain;
		String		path;
		bool		secure;
		bool		httpOnly;

		Cookie(void);
		Cookie(const String &name, const String &value);
		~Cookie(void);
		Cookie(const Cookie &obj);
		Cookie	&operator=(const Cookie &obj);

		bool	operator==(const Cookie &obj);
};

bool	isCookieString(const String &line, std::map<String, Cookie> &cookies);
bool	isCookiePair(const String &line);
bool	isCookieValue(const String &line);
bool	isCookieOctet(const unsigned &character);

void	constructSetCookieHeader(Response &response, const std::vector<Cookie> &cookies);

#endif
