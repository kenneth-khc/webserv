/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBody.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:26:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/30 21:47:09 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEBODY_HPP
#define MESSAGEBODY_HPP

#include <string>
#include <map>
#include <vector>

class	MessageBodyChunk
{
	std::map<std::string,std::string>	contentDisposition;
	std::string	contentType;
	std::string	content;

};

class	MessageBody
{
public:
	MessageBody(const std::string&);

	std::vector<MessageBodyChunk>	chunks;
	std::string						boundary;
	std::string						boundaryEnd;

};

#endif
