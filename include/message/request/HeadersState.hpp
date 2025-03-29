/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersState.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:06:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/29 22:25:51 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERSSTATE_HPP
# define HEADERSSTATE_HPP

# include "RequestState.hpp"

class HeadersState : public RequestState {
	public:
		RequestState	*process(Request &request, Client &client);
};

bool	isFieldValue(const String &line);
bool	isFieldContent(const String &line);
bool	isFieldVisibleCharacter(const unsigned char &character);

#endif
