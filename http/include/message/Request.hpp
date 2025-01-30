/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:03:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 11:17:02 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

# define NUM_OF_METHODS 6
# define NUM_OF_VERSIONS 1

class Request {
	public:
		static const std::string	methods[NUM_OF_METHODS];
		static const float			supportedVersions[NUM_OF_VERSIONS];
		
		int			method;
		std::string	requestTarget;
		float		httpVersion;
		std::map<std::string, std::string>	headers;
		std::string	messageBody;

		Request(void);
		~Request(void);
		Request(const Request &obj);
		Request	&operator=(const Request &obj);
	
		bool		isValidMethod(const std::string &method);
		bool		isSupportedVersion(const float &version);
};

enum METHODS {
	GET,
	HEAD,
	POST,
	PUT,
	OPTIONS,
	DELETE
};

#endif
