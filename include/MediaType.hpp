/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:05:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/26 19:27:14 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include <map>
# include "String.hpp"
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

		class MIMETypesFileError : public std::exception {
				String	message;
			public:
				MIMETypesFileError(const String &message);
				~MIMETypesFileError(void) throw();
				const char	*what(void) const throw();
		};

	public:
		std::map<String, String>	map;

		MediaType(const MediaType &obj);
		MediaType	&operator=(const MediaType &obj);
		MediaType(const String &file);
		~MediaType(void);

		Optional<String>		operator[](const String &key);
		const Optional<String>	operator[](const String &key) const;
};

#endif
