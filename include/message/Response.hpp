/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:44:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/19 23:53:19 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <vector>
# include "String.hpp"
# include "Message.hpp"

# define SERVER_NAME "42webserv"

class Response : public Message {
	public:
		int		statusCode;
		String	reasonPhrase;
		int		flags;

		Response(void);
		~Response(void);
		Response(const Response &obj);
		Response	&operator=(const Response &obj);

		const String	toString(void) const;
		void			setStatusCode(int statusCode);
		void			getFileContents(const String &file);

		enum HeaderFlags {
			CONNECTION_CLOSE = 1
		};

		enum StatusCodes {
			OK = 200,
			CREATED = 201,
			MOVED_PERMANENTLY = 301,
			SEE_OTHER = 303,
			NOT_MODIFIED = 304
		};
};

#endif
