/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestFields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:09:44 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTFIELDS_HPP
# define REQUESTFIELDS_HPP

# include "String.hpp"
# include "Request.hpp"

void	extractFieldLineComponents(Request &request, const String &line);
bool	isFieldValue(const String &line);
bool	isFieldContent(const String &line);
bool	isFieldVisibleCharacter(const unsigned char &character);

#endif
