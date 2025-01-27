/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 00:16:34 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCode.hpp"

ErrorCode::ErrorCode(float httpVersion, int statusCode, std::string reasonPhrase) :
	httpVersion(httpVersion),
	statusCode(statusCode),
	reasonPhrase(reasonPhrase)
{}

const char	*ErrorCode::what(void) const throw() {
	return (this->reasonPhrase.c_str());
}

ErrorCode::~ErrorCode(void) throw() {}

/*******************/
/* 400 Bad Request */
/*******************/
BadRequest400::BadRequest400(void) : 
	ErrorCode(1.1, 400, "Bad Request")
{}

BadRequest400::~BadRequest400(void) throw() {}

/***********************/
/* 501 Not Implemented */
/***********************/
NotImplemented501::NotImplemented501(void) : 
	ErrorCode(1.1, 501, "Not Implemented")
{}

NotImplemented501::~NotImplemented501(void) throw() {}

/*****************/
/* 404 Not Found */
/*****************/
NotFound404::NotFound404(void) :
	ErrorCode(1.1, 404, "Not Found")
{}

NotFound404::~NotFound404(void) throw() {}
