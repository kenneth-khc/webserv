/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:25:04 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 22:03:34 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCODE_HPP
# define ERRORCODE_HPP

# include <exception>
# include <string>

class ErrorCode : public std::exception {
		ErrorCode(const ErrorCode &obj);
		ErrorCode	&operator=(const ErrorCode &obj);
	protected:
		float		httpVersion;
		int			statusCode;
		std::string	reasonPhrase;
		std::string	jsonObject;

		ErrorCode(void);
		~ErrorCode(void) throw();
		ErrorCode(int httpVersion, int statusCode, std::string reasonPhrase);
	public:
		const std::string	toString(void) const;
		const char			*what(void) const throw();
};

class BadRequest400 : public ErrorCode {
	public:
		BadRequest400(void);
		~BadRequest400(void) throw();
};

class NotImplemented501 : public ErrorCode {
	public:
		NotImplemented501(void);
		~NotImplemented501(void) throw();
};


#endif
