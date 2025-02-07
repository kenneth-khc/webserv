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

using std::map;

class	MessageBodyChunk
{
public:
	map<String,String>	contentDisposition;
	String				contentType;
	String				content;

	void	extractKeyValue(const String&);
};

class	MessageBody
{
public:
	MessageBody(const Request&);
	void	parseMultipartFormData();

	std::vector<MessageBodyChunk>	chunks;

private:
	map<String,String>	urlEncodedKeyValues;
	String				contentType;
	String				dashBoundary;
	String				closeBoundary;
	String				body;
};

#endif
