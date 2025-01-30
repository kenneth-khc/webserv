/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:29:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 23:05:51 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <cstdlib>
# include <limits>
# include <string>
# include <map>

class Message {
	public:
		float		httpVersion;
		std::map<std::string, std::string>	headers;
		std::string	messageBody;
		
		Message(void);
		~Message(void);
		Message(const Message &obj);
		Message	&operator=(const Message &obj);

		template<typename type>
		type		find(const std::string &key) const;
		void		insert(const std::string &key, const std::string &value);
		std::string	operator[](const std::string &key);
};

template<typename type>
type		Message::find(const std::string &key) const {}

template<>
std::string	Message::find(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return ("");
	return (it->second);
}

template<>
int	Message::find(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return (std::numeric_limits<int>::min());
	return (std::atoi(it->second.c_str()));
}

template<>
float	Message::find(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return (std::numeric_limits<float>::min());
	return (std::atof(it->second.c_str()));
}

#endif
