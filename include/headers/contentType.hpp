/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentType.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:07:30 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/16 23:06:34 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTENTTYPE_HPP
# define CONTENTTYPE_HPP

# include "String.hpp"
# include "Response.hpp"
# include "MediaType.hpp"

void	constructContentTypeHeader(const String &file, const MediaType &MIMEMappings, Response &response);

#endif
