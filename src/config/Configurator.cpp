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

#include <stdexcept>
#include <utility>
#include "Configurator.hpp"
#include "ConfigErrors.hpp"
#include "Validator.hpp"

/* TODO: this is where all the supported directives go, being mapped to a
 * Validator that determines how to parse/validate the parameters of each
 * Directive */
Configurator::Configurator()
{
	support("prefix", validatePrefix);
	support("http", validateHTTP);
	support("server", validateServer);
	support("listen", validateListen);
	support("location", validateLocation);
	support("root", validateRoot);
	support("index", validateIndex);
	support("types", validateTypes);
	support("server_name", validateServerName);
	support("alias", no_op);
	support("autoindex", validateAutoindex);
	support("allow_method", validateAllowMethod);
	support("client_max_body_size", validateClientMaxBodySize);
	support("error_page", validateErrorPage);
	support("exec_CGI", validateExecCGI);
	support("CGI_script", validateCGIScript);
	support("accept_uploads", validateAcceptUploads);
	support("upload_directory", validateUploadDirectory);
	support("CGI_directory", validateCGIDirectory);
}

void	Configurator::support(const String& name, const Validator& validator)
{
	std::pair<String,Validator> directiveValidator
								= std::make_pair(name, validator);
	supportedDirectives.insert(directiveValidator);
}

/*void	Configurator::validate(const Directive& directive,*/
/*							   const std::multimap<String,Directive>& mappings)*/
/*{*/
/*	const Validator&	validator = operator[](directive.name);*/
/*	validator(directive, mappings);*/
/*}*/

void	Configurator::validate(const Directive* directive,
							   const std::multimap<String,Directive*>& mappings)
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

/* Index into the Configurator, returning a Validator,
 * throwing a std::out_of_range if said Validator cannot be found, meaning
 * we don't support such a directive */
const Validator&	Configurator::operator[](const String& key) const
{
	return supportedDirectives.at(key);
}

void	Configurator::add(Directive* directive, Configuration& config) const
{
	config.add(directive);
}

