/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:02:42 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:38:21 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_HPP
# define PATH_HPP

# include "String.hpp"

bool	isPathAbEmpty(const String &line);
bool	isPathAbsolute(const String &line);
bool	isPathNoScheme(const String &line);
bool	isPathRootless(const String &line);
bool	isPathEmpty(const String &line);
bool	isAbsolutePath(const String &line);

bool	isSegment(const String &line);
bool	isSegmentNZ(const String &line);
bool	isSegmentNZNC(const String &line);
bool	isPrintableCharacter(const String &line, std::size_t &index);

#endif