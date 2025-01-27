/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:25:04 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 00:14:39 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCODE_HPP
# define ERRORCODE_HPP

# include <exception>
# include <string>

class ErrorCode : public std::exception {
	public:
		ErrorCode(const ErrorCode &obj);
		ErrorCode	&operator=(const ErrorCode &obj);
		float		httpVersion;
		int			statusCode;
		std::string	reasonPhrase;
		std::string	jsonObject;

		ErrorCode(void);
		~ErrorCode(void) throw();
		ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase);
	public:
		const std::string	toString(void) const;
		const char			*what(void) const throw();
};

class BadRequest400 : public ErrorCode {
	public:
		BadRequest400(void);
		virtual ~BadRequest400(void) throw();
};

class NotImplemented501 : public ErrorCode {
	public:
		NotImplemented501(void);
		virtual ~NotImplemented501(void) throw();
};

class NotFound404 : public ErrorCode {
	public:
		NotFound404(void);
		virtual ~NotFound404(void) throw();
};


#endif
