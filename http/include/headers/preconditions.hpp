/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preconditions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:20:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/06 21:40:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRECONDITIONS_HPP
# define PRECONDITIONS_HPP

# include <string>
# include <time.h>
# include <sys/stat.h>
# include "Request.hpp"

bool	processPreconditions(const Request &request, const struct stat &statbuf);

bool	processIfNoneMatchHeader(const std::string &value, const std::string &etag);
bool	processIfModifiedHeader(const std::string &dateSince, const struct timespec &lastModified);

#endif
