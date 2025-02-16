/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:54:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/16 17:10:16 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include <map>
#include "String.hpp"
#include "Validator.hpp"

struct	ConfigValidator
{
	ConfigValidator();

	void	support(const String&, const Validator&);
	void	validate(const Directive&);

	/* Returns the Validator for the given Directive */
	const Validator&	operator[](const String&) const;

	std::map<String,Validator>	directives;
};

#endif
