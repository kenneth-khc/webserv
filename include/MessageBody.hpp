/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:26:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/10 01:12:58 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEBODY_HPP
#define MESSAGEBODY_HPP

#include <map>
#include <vector>
#include "String.hpp"
#include "Request.hpp"

using std::map;
using std::vector;

class	MessageBodyPart
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

	vector<MessageBodyPart>	parts;
	String					contentType;

private:
	void	parseURLEncoded();
	void	parseMultipartFormData(String&);

	map<String,String>	urlEncodedKeyValues;
	String				dashBoundary;
	String				closeBoundary;
	String				body;
};

#endif
