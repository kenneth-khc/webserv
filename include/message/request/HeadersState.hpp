/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersState.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:06:53 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/30 03:30:06 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERSSTATE_HPP
# define HEADERSSTATE_HPP

# include "RequestState.hpp"

class HeadersState : public RequestState {
	public:
		RequestState	*process(Request &request, Client &client);
		int				getState(void) const;
};

bool	isFieldValue(const String &line);
bool	isFieldContent(const String &line);
bool	isFieldVisibleCharacter(const unsigned char &character);

#endif
