/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:13:16 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/30 22:34:01 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP

# include <string>
# include "Request.hpp"

void	extractRequestLineComponents(const std::string &line, Request &request);

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const std::string &line);
bool	isOriginForm(const std::string &line);
bool	isAbsoluteForm(const std::string &line);
bool	isAuthorityForm(const std::string &line);
bool	isAsteriskForm(const std::string &line);

#endif
