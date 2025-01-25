/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fields.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/26 03:29:16 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIELDS_HPP
# define FIELDS_HPP

# include <string>

bool	isFieldLine(const std::string &line);
bool	isFieldValue(const std::string &line);
bool	isFieldContent(const std::string &line);
bool	isFieldVisibleCharacter(const unsigned char &character);

#endif
