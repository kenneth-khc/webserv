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

/* TODO: this is where all the supported directives go, being mapped to a
 * Validator that determines how to parse/validate the parameters of each
 * Directive */
Validators::Validators()
{
	registerDirective("prefix", validatePrefix);
	registerDirective("http", validateHTTP);
	registerDirective("server", validateServer);
	registerDirective("listen", validateListen);
	registerDirective("location", validateLocation);
	registerDirective("root", validateRoot);
	registerDirective("index", validateIndex);
	registerDirective("types", validateTypes);
	registerDirective("server_name", validateServerName);
	registerDirective("alias", no_op);
	registerDirective("autoindex", validateAutoindex);
	registerDirective("allow_method", validateAllowMethod);
	registerDirective("client_max_body_size", validateClientMaxBodySize);
	registerDirective("error_page", validateErrorPage);
	registerDirective("exec_CGI", validateExecCGI);
	registerDirective("CGI_script", validateCGIScript);
	registerDirective("accept_uploads", validateAcceptUploads);
	registerDirective("upload_directory", validateUploadDirectory);
	registerDirective("CGI_directory", validateCGIDirectory);
}

void	Validators::registerDirective(const String& name,
									  const Validator& validator)
{
	validators.insert(std::make_pair(name, validator));
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

