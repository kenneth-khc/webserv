/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:03:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 15:44:42 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <sys/socket.h>
# include "String.hpp"
# include "Message.hpp"

# define NUM_OF_METHODS 6
# define NUM_OF_VERSIONS 1

class Request : public Message {
	public:
		static const String	methods[NUM_OF_METHODS];
		static const float	supportedVersions[NUM_OF_VERSIONS];

		int		method;
		String	requestTarget;
		int		flags;
		bool	requestLineFound;
		bool	headersFound;

		Request(void);
		~Request(void);
		Request(const Request &obj);
		Request	&operator=(const Request &obj);

		bool	isValidMethod(const String &method);
		bool	isSupportedVersion(const float &version);
		void	parseRequestLine(String &line);
		void	parseHeaders(String &line);

		enum Methods {
			GET,
			HEAD,
			POST,
			PUT,
			OPTIONS,
			DELETE
		};
};

#endif
