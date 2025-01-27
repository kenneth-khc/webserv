/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 22:02:19 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCode.hpp"

ErrorCode::ErrorCode(int httpVersion, int statusCode, std::string reasonPhrase) :
	httpVersion(httpVersion),
	statusCode(statusCode),
	reasonPhrase(reasonPhrase)
{}

const char	*ErrorCode::what(void) const throw() {
	return (this->reasonPhrase.c_str());
}

/*******************/
/* 400 Bad Request */
/*******************/
BadRequest400::BadRequest400(void) : 
	ErrorCode(1.1, 400, "Bad Request")
{}

/***********************/
/* 501 Not Implemented */
/***********************/
NotImplemented501::NotImplemented501(void) : 
	ErrorCode(1.1, 501, "Not Implemented")
{}
