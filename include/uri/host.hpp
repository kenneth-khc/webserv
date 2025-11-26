/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:48:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/12 02:04:28 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOST_HPP
# define HOST_HPP

# include "String.hpp"

bool	isHost(const String &line);
bool	isIPLiteral(const String &line);
bool	isIPV6(const String &line);
bool	isIPVFuture(const String &line);
bool	isIPV4(const String &line);

bool	is16BitsHexa(const String &line);
bool	isDecOctet(const String &line);
bool	isRegName(const String &line);

#endif
