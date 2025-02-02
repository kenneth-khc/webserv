/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ifModified.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:56:27 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/04 03:57:06 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IFMODIFIED_HPP
# define IFMODIFIED_HPP

# include <string>
# include <ctime>

bool		processIfModifiedHeader(const std::string &dateSince, const struct timespec &lastModified);
std::string	convertLastModifiedToHTTPDate(const struct timespec &lastModified);

#endif
