/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:54:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/15 05:09:53 by kecheong         ###   ########.fr       */
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

	void	add(const String&, const Validator&);
	const Validator&	operator[](const String&) const;

	std::map<String,Validator>	directives;
};

#endif
