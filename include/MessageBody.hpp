/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:26:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 23:15:19 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEBODY_HPP
#define MESSAGEBODY_HPP

#include <string>
#include <map>
#include <vector>
#include "String.hpp"
#include "Request.hpp"

class	MessageBodyChunk
{
public:
	std::map<String,String>	contentDisposition;
	String					contentType;
	String					content;

};

class	MessageBody
{
public:
	MessageBody(const Request&);
	void	parseMultipartFormData();

private:
	String							contentType;
	String							dashBoundary;
	String							closeBoundary;
	String							body;
	std::vector<MessageBodyChunk>	chunks;

};

bool	isCR(char c);
bool	isLF(char c);
bool	isCRLF(const std::string& s);
bool	isSpace(char c);
bool	isHTab(char c);
bool	isWhiteSpace(char c);
bool	isLinearWhiteSpace(const std::string& s);

#endif
