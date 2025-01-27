/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:25:04 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 05:02:34 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCODE_HPP
# define ERRORCODE_HPP

# include <exception>
# include <string>
# include "Response.hpp"

class ErrorCode : public std::exception, public Response {
		ErrorCode	&operator=(const ErrorCode &obj);
		ErrorCode(void);

	protected:
		~ErrorCode(void) throw();
		ErrorCode(const ErrorCode &obj);
		ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase);
		ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase, const std::string &title);

	public:
		std::string	title;
};

class BadRequest400 : public ErrorCode {
	public:
		BadRequest400(void);
		BadRequest400(std::string title);
};

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
