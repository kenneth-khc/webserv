/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 03:00:41 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/22 05:26:42 by kecheong         ###   ########.fr       */
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

	/** A (const Directive&, const Map&) -> void function */
	typedef void (*ValidationFunc)(const Directive&, const Directive::Map&);

	/** Construct a Validator using a ValidationFunc to validate a Directive */
	Validator(ValidationFunc);

	/** Call operator to invoke the underlying ValidationFunc */
	void	operator()(const Directive&, const Directive::Map&) const;

private:

	/** The underlying function pointer to invoke for validation */
	void	(*function)(const Directive&, const Directive::Map&);
};

void	no_op(const Directive&, const Directive::Map&);
void	validatePrefix(const Directive&, const Directive::Map&);
void	validateListen(const Directive&, const Directive::Map&);
void	validateHTTP(const Directive&, const Directive::Map&);
void	validateServer(const Directive&, const Directive::Map&);
void	validateServerName(const Directive&, const Directive::Map&);
void	validateLocation(const Directive&, const Directive::Map&);
void	validateRoot(const Directive&, const Directive::Map&);
void	validateIndex(const Directive&, const Directive::Map&);
void	validateTypes(const Directive&, const Directive::Map&);
void	validateAutoindex(const Directive&, const Directive::Map&);
void	validateAllowMethod(const Directive&, const Directive::Map&);
void	validateClientMaxBodySize(const Directive&, const Directive::Map&);
void	validateErrorPage(const Directive&, const Directive::Map&);
void	validateExecCGI(const Directive&, const Directive::Map&);
void	validateCGIScript(const Directive&, const Directive::Map&);
void	validateAcceptUploads(const Directive&, const Directive::Map&);
void	validateUploadDirectory(const Directive&, const Directive::Map&);
void	validateCGIDirectory(const Directive&, const Directive::Map&);

#endif
