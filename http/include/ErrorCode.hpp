/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:25:04 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/07 22:31:50 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ERRORCODE_HPP
# define ERRORCODE_HPP

# include <exception>
# include <string>
# include "Response.hpp"

class ErrorCode : public std::exception, public Response {
	public:
		std::string	title;

		ErrorCode(void);
		ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase);
		ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase, const std::string &title);
		~ErrorCode(void) throw();
		ErrorCode(const ErrorCode &obj);
		ErrorCode	&operator=(const ErrorCode &obj);
};

/********************/
/* Client Error 4XX */
/********************/
class BadRequest400 : public ErrorCode {
	public:
		BadRequest400(void);
		BadRequest400(std::string title);
};

class NotFound404 : public ErrorCode {
	public:
		NotFound404(void);
		NotFound404(std::string title);
};

class PreconditionFailed412 : public ErrorCode {
	public:
		PreconditionFailed412(void);
		PreconditionFailed412(std::string title);
};

/********************/
/* Server Error 5XX */
/********************/
class NotImplemented501 : public ErrorCode {
	public:
		NotImplemented501(void);
		NotImplemented501(std::string title);
};

class VersionNotSupported505 : public ErrorCode {
	public:
		VersionNotSupported505(void);
		VersionNotSupported505(std::string title);
};

#endif
