/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIMessageBodyState.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:16:02 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/02 22:50:13 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIMessageBodyState.hpp"

CGIOutputState	*CGIMessageBodyState::parse(
Response &response,
String &output,
ssize_t bytes)
{
	(void)bytes;
	response.messageBody.append(output.c_str(), output.length());
	output.erase(0, output.length());
	return (this);
}
