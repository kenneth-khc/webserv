/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminalValues.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:06:55 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/26 02:53:00 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINALVALUES_HPP
# define TERMINALVALUES_HPP

# include <string>

bool	isToken(const std::string &line);
bool	isUnreservedCharacter(const unsigned char &character);
bool	isPercentEncoded(const std::string &line, int index);
bool	isSubDelimiter(const unsigned char &character);
bool	isVisibleCharacter(const unsigned char &character);

bool	isObsoleteText(const unsigned char &character);

#endif