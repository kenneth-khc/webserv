/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:55:29 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/08 19:19:48 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "Request.hpp"
# include "Response.hpp"

# define TIMEOUT_VALUE 15

void	constructConnectionHeader(const Request &request, Response &response);

#endif
