/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:13:16 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/14 22:10:11 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP

# include "String.hpp"
# include "Request.hpp"

void	extractRequestLineComponents(Request &request, const String &line);

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const String &line);
bool	isOriginForm(const String &line);
bool	isAbsoluteForm(const String &line);
bool	isAuthorityForm(const String &line);
bool	isAsteriskForm(const String &line);

#endif
