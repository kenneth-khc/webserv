/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   host.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:48:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 01:22:58 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOST_HPP
# define HOST_HPP

# include <string>

bool	isHost(const std::string &line);
bool	isIPLiteral(const std::string &line);
bool	isIPV6(const std::string &line);
bool	isIPVFuture(const std::string &line);
bool	isIPV4(const std::string &line);

bool	is16BitsHexa(const std::string &line);
bool	isDecOctet(const std::string &line);
bool	isRegName(const std::string &line);

#endif
