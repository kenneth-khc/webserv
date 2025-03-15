/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:41:03 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/15 22:57:32 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location():
	matchType(PREFIX),
	root("html"),
	alias(),
	autoindex(false),
	MIMEMappings("mime.types")
{

}
