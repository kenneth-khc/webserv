/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:13:16 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 01:26:32 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP

#include <string>

bool	isRequestLine(const std::string &line);

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const std::string &line);
bool	isOriginForm(const std::string &line);
bool	isAbsoluteForm(const std::string &line);
bool	isAuthorityForm(const std::string &line);
bool	isAsteriskForm(const std::string &line);


#endif
