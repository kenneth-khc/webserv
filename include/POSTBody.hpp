/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTBody.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:26:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/05 17:53:24 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTBODY_HPP
#define POSTBODY_HPP

#include <map>
#include <vector>
#include "String.hpp"
#include "Request.hpp"

using std::map;
using std::vector;

class	POSTBodyPart
{
public:
	map<String,String>	contentDisposition;
	String				contentType;
	String				content;

	void	extractKeyValue(const String&);
};

class	POSTBody
{
public:
	POSTBody(const Request&);

	vector<POSTBodyPart>	parts;
	String					contentType;
	map<String, String>		urlEncodedKeyValues;
	// std::map sorts the keys so we use also use vectors to preserve order
	vector<String>			urlEncodedKeys;
	vector<String>			urlEncodedValues;

private:
	void	parseURLEncoded();
	void	parseMultipartFormData(String&);

	String				dashBoundary;
	String				closeBoundary;
	String				body;
};

#endif
