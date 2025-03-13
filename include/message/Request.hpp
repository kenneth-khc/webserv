/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:03:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/13 07:38:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include "Optional.hpp"
# include "String.hpp"
# include "Message.hpp"
# include "Cookie.hpp"

# define NUM_OF_METHODS 4
# define NUM_OF_VERSIONS 1

class Client;

class Request : public Message {
	public:
		static const String	methods[NUM_OF_METHODS];
		static const float	supportedVersions[NUM_OF_VERSIONS];

		Client						*client;

		bool						requestLineFound;
		String						method;
		String						requestTarget;
		String						absolutePath;
		Optional<String>			query;
		Optional<String>			decodedQuery;
		std::map<String, String>	queryPairs;

		bool						headersFound;
		std::map<String, Cookie>	cookies;

		bool						hasMessageBody;
		bool						messageBodyFound;
		//	For chunked message body
		String::size_type			chunkSize;
		String::size_type			length;
		bool						chunkSizeFound;
		bool						lastChunk;

		Request(void);
		~Request(void);
		Request(const Request &obj);
		Request	&operator=(const Request &obj);

		bool	isValidMethod(const String &method);
		bool	isSupportedVersion(const float &version);
		void	parseRequestLine(String &line);
		void	parseHeaders(String &line);
		void	parseMessageBody(String &line);
		void	parseCookieHeader(void);
};

#endif
