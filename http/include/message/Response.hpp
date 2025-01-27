/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:44:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 18:46:58 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

typedef struct Response {
		int			statusCode;
		float		httpVersion;
		std::string	reasonPhrase;

		std::map<std::string, std::string>	headers;

		Response(void);
		~Response(void);
	private:
		Response(const Response &obj);
		Response	&operator=(const Response &obj);
}	Response;

#endif
