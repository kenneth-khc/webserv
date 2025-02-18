/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:25:04 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/18 03:08:10 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ERRORCODE_HPP
# define ERRORCODE_HPP

# include <exception>
# include "String.hpp"
# include "Response.hpp"

class ErrorCode : public std::exception, public Response {
	public:
		ErrorCode(void);
		ErrorCode(float httpVersion, int statusCode, String reasonPhrase);
		ErrorCode(float httpVersion, int statusCode, String reasonPhrase, const char *title);
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
		BadRequest400(const char *title);
};

class NotFound404 : public ErrorCode {
	public:
		NotFound404(void);
		NotFound404(const char *title);
};

class PreconditionFailed412 : public ErrorCode {
	public:
		PreconditionFailed412(void);
		PreconditionFailed412(const char *title);
};

class UnsupportedMediaType415 : public ErrorCode {
	public:
		UnsupportedMediaType415(void);
		UnsupportedMediaType415(const char *title);
		UnsupportedMediaType415(const char *header, const char *value);
};

/********************/
/* Server Error 5XX */
/********************/
class NotImplemented501 : public ErrorCode {
	public:
		NotImplemented501(void);
		NotImplemented501(const char *title);
};

class VersionNotSupported505 : public ErrorCode {
	public:
		VersionNotSupported505(void);
		VersionNotSupported505(const char *title);
};

#endif
