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

#include <map>
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
	Validator(void (*)(const Directive&, const std::multimap<String,Directive>&));

	// Call operator to invoke the underlying function
	void	operator()(const Directive&, const std::multimap<String,Directive>&) const;

	// TODO: testing this. validators require the surrounding mappings
	//		 to know the frequency of the directive
private:
	void	(*function)(const Directive&, const std::multimap<String,Directive>&);
};

typedef std::multimap<String,Directive> Mappings;

void	no_op(const Directive&);
void	validatePrefix(const Directive&, const Mappings&);
void	validateListen(const Directive&, const Mappings&);
void	validateHTTP(const Directive&, const Mappings&);
void	validateServer(const Directive&, const Mappings&);
void	validateServerName(const Directive&, const Mappings&);
void	validateLocation(const Directive&, const Mappings&);
void	validateRoot(const Directive&, const Mappings&);
void	validateIndex(const Directive&, const Mappings&);
void	validateTypes(const Directive&, const Mappings&);

#endif
