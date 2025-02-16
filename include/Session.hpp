/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 22:45:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/16 04:13:35 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
# define SESSION_HPP

# include <vector>
# include "String.hpp"
# include "Optional.hpp"
# include "Cookie.hpp"

# define NUM_OF_COOKIES 1

class Session {
		static const String	allowedCopyableCookies[NUM_OF_COOKIES];
	public:
		std::vector<Cookie>	cookies;

		Session(void);
		~Session(void);
		Session(const Session &obj);
		Session	&operator=(const Session &obj);

		Optional<String>					operator[](const String &cookieName);
		const Optional<String>				operator[](const String &cookieName) const;
		std::vector<Cookie>::iterator		find(const String &key);
		std::vector<Cookie>::const_iterator	find(const String &key) const;
		bool								operator==(const Session &obj) const;
		bool								operator!=(const Session &obj) const;

		std::vector<Cookie>					updateSession(const Session &obj);
};

#endif
