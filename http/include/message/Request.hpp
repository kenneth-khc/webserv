/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:03:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 13:16:51 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <sys/socket.h>
# include "Message.hpp"

# define NUM_OF_METHODS 6
# define NUM_OF_VERSIONS 1

class Request : public Message {
	public:
		static const std::string	methods[NUM_OF_METHODS];
		static const float			supportedVersions[NUM_OF_VERSIONS];

		int					method;
		std::string			requestTarget;
		int					socketFD;
		sockaddr_storage	srcAddress;
		int					flags;

		Request(void);
		~Request(void);
		Request(const Request &obj);
		Request	&operator=(const Request &obj);

		bool	isValidMethod(const std::string &method);
		bool	isSupportedVersion(const float &version);
		void	parseRequestLine(std::string &line);
		void	parseHeaders(std::string &line);

		enum Methods {
			GET,
			HEAD,
			POST,
			PUT,
			OPTIONS,
			DELETE
		};

		enum HeaderFlags {
			IFMODIFIED = 1,
			CONNECTION = 1 << 1
		};
};

#endif
