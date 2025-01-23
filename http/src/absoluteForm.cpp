/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absoluteForm.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:20:31 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/24 22:28:36 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "absoluteForm.hpp"

bool	isAbsoluteForm(const std::string &line) {
	if (isAbsoluteURI(line) == true)
		return (true);
	return (false);
}
