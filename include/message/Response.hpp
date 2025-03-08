/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:44:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/05 17:20:58 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <vector>
# include "String.hpp"
# include "Message.hpp"

class ErrorCode;

class Response : public Message {
	public:
		int		statusCode;
		String	reasonPhrase;
		int		flags;

		Response(void);
		~Response(void);
		Response(const Response &obj);
		Response	&operator=(const Response &obj);
		Response	&operator=(const ErrorCode &obj);

		const String	toString(void) const;
		void			setStatusCode(int statusCode);
		void			getFileContents(const String &file);

		enum HeaderFlags {
			CONNECTION_CLOSE = 1
		};

		enum StatusCodes {
			OK = 200,
			MOVED_PERMANENTLY = 301,
			SEE_OTHER = 303,
			NOT_MODIFIED = 304,
			BAD_REQUEST = 400,
			NOT_FOUND = 404,
			PRECONDITION_FAILED = 412,
			UNSUPPORTED_MEDIA_TYPE = 415,
			INTERNAL_SEVER_ERROR = 500,
			NOT_IMPLEMENTED = 501,
			VERSION_NOT_SUPPORTED = 505
		};
};

#endif
