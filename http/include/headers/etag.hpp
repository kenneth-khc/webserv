/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:21:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/08 12:39:12 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ETAG_HPP
# define ETAG_HPP

# include <string>
# include <time.h>
# include "Optional.hpp"

bool	isETagHeader(const std::string &line);
bool	isEntityTag(const std::string &line);
bool	isOpaqueTag(const std::string &line);
bool	isEntityTagCharacter(const std::string &line);

std::string	constructETagHeader(const struct timespec &lastModified, Optional<int> contentLength);
bool		compareETagStrong(const std::string &etagOne, const std::string &etagTwo);
bool		compareETagWeak(std::string etagOne, std::string etagTwo);

#endif
