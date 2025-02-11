/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:02:42 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 04:38:05 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_HPP
# define PATH_HPP

# include <string>

bool	isPathAbEmpty(const std::string &line);
bool	isPathAbsolute(const std::string &line);
bool	isPathNoScheme(const std::string &line);
bool	isPathRootless(const std::string &line);
bool	isPathEmpty(const std::string &line);
bool	isAbsolutePath(const std::string &line);

bool	isSegment(const std::string &line);
bool	isSegmentNZ(const std::string &line);
bool	isSegmentNZNC(const std::string &line);
bool	isPrintableCharacter(const std::string &line, std::size_t &index);

#endif