/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:03:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 19:14:19 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

# define NUM_OF_METHODS 6

typedef struct Request {
		static const std::string	methods[NUM_OF_METHODS];
		
		int			method;
		bool		isValidMethod(const std::string &method);
		std::string	requestTarget;
		float		httpVersion;

		std::map<std::string, std::string>	headers;

		Request(void);
		~Request(void);
	private:
		Request(const Request &obj);
		Request	&operator=(const Request &obj);
}	Request;

enum METHODS {
	GET,
	HEAD,
	POST,
	PUT,
	OPTIONS,
	DELETE
};

#endif
