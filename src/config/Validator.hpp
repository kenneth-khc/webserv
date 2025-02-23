/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 03:00:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/23 22:40:20 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include "Directive.hpp"

/* A Validator is a function object that validates directives.
 * The idea is that the rules and contraints of a directive gets stored here,
 * and each supported directive is associated with its own Validator.
 * When a directive is parsed, we find it in our list of supported directives
 * and call its Validator to ensure the parameters are valid */

class	Validator
{
public:
	// Construct a Validator taking in a Directive->void function used to
	// validate the Directive
	Validator(void (*)(const Directive&));

	// Call operator to invoke the underlying function
	void	operator()(const Directive&) const;

private:
	void	(*function)(const Directive&);
};

void	no_op(const Directive&);
void	validatePrefix(const Directive&);
void	validateListen(const Directive&);
void	validateHTTP(const Directive& );
void	validateServer(const Directive&);
void	validateLocation(const Directive&);
void	validateRoot(const Directive&);
void	validateIndex(const Directive&);

#endif
