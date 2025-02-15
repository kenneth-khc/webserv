/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:55:07 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 06:00:14 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigValidator.hpp"
#include "Validator.hpp"

/* TODO: this is where all the supported directives go, being mapped to a
 * Validator that determines how to parse/validate the parameters of each
 * Directive */
ConfigValidator::ConfigValidator()
{
	add("prefix", checkIfAbsolutePath);
	add("key", returnsTrue);
	add("http", validateHTTP);
	add("server", returnsTrue);
	add("listen", isValidPortNumber);
}
