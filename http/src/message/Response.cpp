/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:20:18 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/28 00:44:49 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void):
statusCode(0),
httpVersion(1.1),
reasonPhrase(),
headers(),
body()
{

}

Response::~Response() {}

Response::Response(const Response& other):
statusCode(other.statusCode),
httpVersion(other.httpVersion),
reasonPhrase(other.reasonPhrase),
headers(other.headers),
body(other.body)
{

}

Response&	Response::operator=(const Response& other)
{
	this->statusCode = other.statusCode;
	this->httpVersion = other.httpVersion;
	this->reasonPhrase = other.reasonPhrase;
	this->headers = other.headers;
	this->body = other.body;

	return *this;
}
