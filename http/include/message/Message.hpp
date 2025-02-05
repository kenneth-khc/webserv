/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:29:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 17:19:22 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <cstdlib>
# include <limits>
# include <string>
# include <map>
# include "Optional.hpp"

# define NUM_OF_HEADERS 22

class Message {
	public:
		static const std::string	allowedDuplicateHeaders[NUM_OF_HEADERS];
		float		httpVersion;
		std::map<std::string, std::string>	headers;
		std::string	messageBody;

		Message(void);
		~Message(void);
		Message(const Message &obj);
		Message	&operator=(const Message &obj);

		template<typename type>
		type	find(const std::string &key) const;

		void	insert(const std::string &key, const std::string &value);
		void	insert(const std::string &key, const int &value);

		Optional<std::string>		operator[](const std::string &key);
		const Optional<std::string>	operator[](const std::string &key) const;
};

template<>
inline Optional<std::string>	Message::find< Optional<std::string> >(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return Optional<std::string>();
	return (it->second);
}

template<>
inline Optional<int>	Message::find< Optional<int> >(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return Optional<int>();
	return (std::atoi(it->second.c_str()));
}

template<>
inline Optional<float>	Message::find< Optional<float> >(const std::string &key) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = this->headers.find(key);
	if (it == this->headers.end())
		return (Optional<float>());
	return (std::atof(it->second.c_str()));
}

#endif
