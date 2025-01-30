/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:44:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 11:16:27 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

# define SERVER_NAME "42webserv"

class Response {
	public:
		float		httpVersion;
		int			statusCode;
		std::string	reasonPhrase;
		std::map<std::string, std::string>	headers;
		std::string	messageBody;

		Response(void);
		~Response(void);
		Response(const Response &obj);
		Response	&operator=(const Response &obj);

		const std::string	toString(void) const;
};

#endif
