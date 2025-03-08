/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:49:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:35:49 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
# define HEADERS_HPP

# define NUM_OF_MANDATORY_HEADERS 1

# include "String.hpp"

bool	checkMandatoryHeaders(String fieldName, const String &fieldValue);

#endif
