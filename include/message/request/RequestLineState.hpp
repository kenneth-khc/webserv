/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineState.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:45:20 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/29 22:25:31 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTLINESTATE_HPP
# define REQUESTLINESTATE_HPP

# include "RequestState.hpp"

class RequestLineState : public RequestState {
	public:
		RequestState	*process(Request &request, Client &client);
};

/******************/
/* Request Target */
/******************/
bool	isRequestTarget(const String &line);
bool	isOriginForm(const String &line);
bool	isAbsoluteForm(const String &line);
bool	isAuthorityForm(const String &line);
bool	isAsteriskForm(const String &line);

#endif
