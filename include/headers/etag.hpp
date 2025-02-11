/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:21:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:35:37 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ETAG_HPP
# define ETAG_HPP

# include <time.h>
# include "String.hpp"
# include "Optional.hpp"

bool	isETagHeader(const String &line);
bool	isEntityTag(const String &line);
bool	isOpaqueTag(const String &line);
bool	isEntityTagCharacter(const String &line);

String	constructETagHeader(const struct timespec &lastModified, Optional<int> contentLength);
bool	compareETagStrong(const String &etagOne, const String &etagTwo);
bool	compareETagWeak(String etagOne, String etagTwo);

#endif
