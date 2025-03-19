/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:29:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/20 06:26:12 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <sstream>
# include <map>
# include "String.hpp"
# include "Optional.hpp"

# define NUM_OF_HEADERS 22

class Message {
	public:
		static const String				allowedDuplicateHeaders[NUM_OF_HEADERS];
		float							httpVersion;
		std::multimap<String, String>	headers;
		String							messageBody;

		Message(void);
		~Message(void);
		Message(const Message &obj);
		Message	&operator=(const Message &obj);

		static String	stringToLower(String str);

		template<typename type>
		type	find(const String &key) const;

		void	insert(const String &key, const String &value);
		void	insert(const String &key, const String::size_type &value);

		Optional<String>		operator[](const String &key);
		const Optional<String>	operator[](const String &key) const;
};

template<>
inline Optional<String>	Message::find< Optional<String> >(const String &key) const {
	std::multimap<String, String>::const_iterator	it;
	String	lowercaseKey = Message::stringToLower(key);

	it = this->headers.find(lowercaseKey);
	if (it == this->headers.end())
		return Optional<String>();
	return (it->second);
}

template<>
inline Optional<int>	Message::find< Optional<int> >(const String &key) const {
	std::multimap<String, String>::const_iterator	it;
	String	lowercaseKey = Message::stringToLower(key);

	it = this->headers.find(lowercaseKey);
	if (it == this->headers.end())
		return Optional<int>();

	std::stringstream	stream;
	int					value;

	stream << it->second;
	stream >> value;
	return (value);
}

template<>
inline Optional<String::size_type>	Message::find< Optional<String::size_type> >(const String &key) const {
	std::multimap<String, String>::const_iterator	it;
	String	lowercaseKey = Message::stringToLower(key);

	it = this->headers.find(lowercaseKey);
	if (it == this->headers.end())
		return Optional<String::size_type>();

	std::stringstream	stream;
	String::size_type	value;

	stream << it->second;
	stream >> value;
	return (value);
}

#endif
