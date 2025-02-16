/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:55:07 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 17:21:22 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigValidator.hpp"
#include "ConfigErrors.hpp"
#include "Validator.hpp"
#include <stdexcept>

/* TODO: this is where all the supported directives go, being mapped to a
 * Validator that determines how to parse/validate the parameters of each
 * Directive */
ConfigValidator::ConfigValidator()
{
	/*support("key", returnsTrue);*/
	support("prefix", validatePrefix);
	support("http", validateHTTP);
	support("server", validateServer);
	support("listen", validateListen);
	support("location", validateLocation);
	support("root", validateRoot);
	support("index", validateIndex);
}

void	ConfigValidator::support(const String& name, const Validator& validator)
{
	directives.insert(std::make_pair(name, validator));
}

void	ConfigValidator::validate(const Directive& directive)
{
	const Validator&	validator = operator[](directive.name);
	validator.validate(directive);
}

const Validator&	ConfigValidator::operator[](const String& key) const
{
	try
	{
		const Validator&	validator = directives.at(key);
		return validator;
	}
	catch (const std::out_of_range& e)
	{
		throw InvalidDirective(key);
	}
}

