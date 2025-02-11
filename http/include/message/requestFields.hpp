/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestFields.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 17:41:03 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTFIELDS_HPP
# define REQUESTFIELDS_HPP

# include <string>
# include "Request.hpp"

void	extractFieldLineComponents(const std::string &line, Request &request);
bool	isFieldValue(const std::string &line);
bool	isFieldContent(const std::string &line);
bool	isFieldVisibleCharacter(const unsigned char &character);

#endif
