/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:03:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/19 17:20:23 by cteoh            ###   ########.fr       */
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

		String						method;
		String						requestTarget;
		String						path;
		Optional<String>			query;
		Optional<String>			decodedQuery;
		std::map<String, String>	queryPairs;
		std::map<String, Cookie>	cookies;

		//	For chunked message body
		String::size_type			chunkSize;
		String::size_type			bodyLength;
		bool						chunkSizeFound;
		bool						lastChunk;

		Request(void);
		~Request(void);
		Request(const Request &obj);
		Request	&operator=(const Request &obj);

		void	parseRequestLine(String &line);
		bool	isValidMethod(const String &method);
		bool	isSupportedVersion(const float &version);
		void	parseHeaders(String &line);
		void	parseCookieHeader(void);
		void	checkIfBodyExists(void);
		void	parseMessageBody(String &line);

		enum ProcessStage {
			REQUEST_LINE = 0x001,
			HEADERS = 0x002,
			HEAD_DONE = 0x004,
			MESSAGE_BODY = 0x008,
			READY = 0x010,
			DONE = 0x020
		};
};

#endif
