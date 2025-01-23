/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminalValues.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:06:55 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:28:33 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINALVALUES_HPP
# define TERMINALVALUES_HPP

# include <string>

bool	isToken(const std::string &line);
bool	isUnreservedCharacter(const char &character);
bool	isPercentEncoded(const std::string &line, int index);
bool	isSubDelimiter(const char &character);

#endif