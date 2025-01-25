/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestTarget.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:15:40 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/25 18:39:15 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTTARGET_HPP
# define REQUESTTARGET_HPP

# include <string>

bool	isRequestTarget(const std::string &line);
bool	isOriginForm(const std::string &line);
bool	isAbsoluteForm(const std::string &line);
bool	isAuthorityForm(const std::string &line);
bool	isAsteriskForm(const std::string &line);

#endif
