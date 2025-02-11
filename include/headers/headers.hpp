/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:49:28 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/05 18:03:03 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
# define HEADERS_HPP

# define NUM_OF_MANDATORY_HEADERS 1

# include <string>

bool	checkMandatoryHeaders(std::string fieldName, const std::string &fieldValue);

#endif
