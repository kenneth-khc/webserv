/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preconditions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:20:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:36:02 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRECONDITIONS_HPP
# define PRECONDITIONS_HPP

# include <time.h>
# include <sys/stat.h>
# include "String.hpp"
# include "Request.hpp"

bool	processPreconditions(const Request &request, const struct stat &statbuf);

bool	processIfNoneMatchHeader(const String &value, const String &etag);
bool	processIfModifiedHeader(const String &dateSince, const struct timespec &lastModified);

#endif
