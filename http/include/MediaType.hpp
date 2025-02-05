/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:05:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 03:41:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include <string>
# include <map>
# include "Optional.hpp"

class MediaType {
		MediaType(void);
		MediaType(const MediaType &obj);
		MediaType	&operator=(const MediaType &obj);
	public:
		std::map<std::string, std::string>	map;

		MediaType(const std::string &file);
		~MediaType(void);

		Optional<std::string>		operator[](const std::string &key);
		const Optional<std::string>	operator[](const std::string &key) const;
};

#endif
