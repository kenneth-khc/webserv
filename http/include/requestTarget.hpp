/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestTarget.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:15:40 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:17:19 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTTARGET_HPP
# define REQUESTTARGET_HPP

# include <string>

bool	isOriginForm(const std::string &line);
bool	isAbsoluteForm(const std::string &line);

#endif
