/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:09:44 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/23 23:31:52 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>
# include "enums.h"

# define NUM_OF_METHODS 5

class Request {
		Request(void);
		Request(const Request &obj);
		Request	&operator=(const Request &obj);
	public:
		static const std::string			methods[NUM_OF_METHODS];

		int			method;
		std::string	requestTarget;
		std::string	httpVersion;
		std::map<std::string, std::string>	headers;

		Request(const std::string &message);
		~Request(void);
};

#endif