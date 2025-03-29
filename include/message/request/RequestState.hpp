/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestState.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:43:35 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/31 08:11:52 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTSTATE_HPP
# define REQUESTSTATE_HPP

class Client;
class Request;
class DoneState;

class RequestState {
	public:
		virtual ~RequestState(void);
		virtual RequestState	*process(Request &request, Client &client) = 0;
		virtual int				getState(void) const = 0;

		enum States {
			REQUEST_LINE,
			HEADERS,
			HEAD_DONE,
			MESSAGE_BODY,
			DONE
		};
};

#endif
