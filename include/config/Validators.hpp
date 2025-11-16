/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configurator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:54:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 18:13:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATORS_HPP
#define VALIDATORS_HPP

#include "String.hpp"
#include "Validator.hpp"

#include <map>

struct	Validators
{
public:

	Validators();

	/** Registers a Directive to support, providing the name of the Directive
		as a key and the Validator to use for validating the Directive */
	void	registerDirective(const String&, const Validator&);

	/** Takes in a Directive, searches for its Validator and then calls it.
		Throws an InvalidDirective if the Directive is not supported */
	void	validate(const Directive*, const Directive::Map&) const;

private:

	/** Returns the Validator for the given Directive */
	const Validator&	operator[](const String&) const;

	/** A map of DirectiveName -> Validator */
	std::map<String,Validator>	validators;
};

#endif
