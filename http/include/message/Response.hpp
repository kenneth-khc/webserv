/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:44:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/04 03:15:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <sys/socket.h>
# include "Message.hpp"

# define SERVER_NAME "42webserv"

class Response : public Message {
	public:
		int					statusCode;
		std::string			reasonPhrase;
		int					socketFD;
		sockaddr_storage	destAddress;
		int					flags;

		Response(void);
		~Response(void);
		Response(const Response &obj);
		Response	&operator=(const Response &obj);

		const std::string	toString(void) const;
		void				setStatusCode(int statusCode);

		enum HeaderFlags {
			CONNECTION = 1
		};

		enum StatusCodes {
			OK = 200,
			NOT_MODIFIED = 304
		};
};

#endif
