/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:05:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 05:42:28 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include <string>
# include <map>
# include "Optional.hpp"

/***************************************/
/* File extension to MIME type mapping */
/***************************************/
//	At the start of the program, constructs a map of file extensions to their
//	respective MIME types. The map is based on specifications within a file
//	(defaulted to 'mime.types').
//
//	This map would be used in file retrieval operations such as GET requests to
//	construct the 'Content-Type' header.
class MediaType {
		MediaType(void);

		class MIMETypesNotFound : public std::exception {
			public:
				MIMETypesNotFound(void);
				~MIMETypesNotFound(void) throw();
				const char	*what(void) const throw();
		};

	public:
		std::map<std::string, std::string>	map;

		MediaType(const std::string &file);
		MediaType(const MediaType &obj);
		MediaType	&operator=(const MediaType &obj);
		~MediaType(void);

		Optional<std::string>		operator[](const std::string &key);
		const Optional<std::string>	operator[](const std::string &key) const;
};

#endif
