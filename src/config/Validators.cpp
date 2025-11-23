/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configurator.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:55:07 by kecheong          #+#    #+#             */
/*   Updated: 2025/04/03 21:12:35 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validators.hpp"
#include "InvalidDirective.hpp"
#include "Validator.hpp"

#include <stdexcept>
#include <utility>

Validators::Validators()
{
	// global directives
	registerDirective("prefix", validatePrefix);
	registerDirective("http",
					 Validator(validateHttpHeader, validateHttpBody));

	// http directives
	registerDirective("types", validateTypes);
	registerDirective("server",
					  Validator(validateServerHeader, validateServerBody));

	// server directives
	registerDirective("server_name", validateServerName);
	registerDirective("listen", validateListen);
	registerDirective("location",
					  Validator(validateLocationHeader, validateLocationBody));

	// location directives
	registerDirective("root", validateRoot);
	registerDirective("alias", validateAlias);
	registerDirective("index", validateIndex);
	registerDirective("autoindex", validateAutoindex);
	registerDirective("allow_method", validateAllowMethod);
	registerDirective("client_max_body_size", validateClientMaxBodySize);
	registerDirective("error_page", validateErrorPage);
	registerDirective("upload_directory", validateUploadDirectory);
	registerDirective("redirect", validateRedirect);

	// cgi_script directives
	registerDirective("cgi_script",
	                  Validator(validateCgiScriptHeader, validateCgiScriptBody));
	registerDirective("cgi_bin_directory", validateCgiBinDirectory);
	registerDirective("cgi_upload_directory", validateCgiUploadDirectory);
}

void	Validators::registerDirective(const String& name,
									  const Validator& validator)
{
	validators.insert(std::make_pair(name, validator));
}

const Validator&	Validators::getValidator(const Directive* directive) const
{
	try
	{
		const Validator& validator =
		operator[](directive->name);
		return validator;
	}
	catch (const std::out_of_range&)
	{
		throw InvalidDirective(directive);
	}
}

void	Validators::validate(const Directive* directive,
							 const Directive::Map& mappings) const
{
	try
	{
		const Validator&	validator = operator[](directive->name);
		validator(*directive, mappings);
	}
	catch (const std::out_of_range& e)
	{
		throw InvalidDirective(directive);
	}
}

/* Index into the Validators, returning a Validator,
 * throwing a std::out_of_range if said Validator cannot be found, meaning
 * we don't support such a directive */
const Validator&	Validators::operator[](const String& key) const
{
	return validators.at(key);
}

