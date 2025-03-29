/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestState.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:43:35 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/29 23:14:08 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTSTATE_HPP
# define REQUESTSTATE_HPP

# include "String.hpp"

class Client;
class Request;

class RequestState {
	public:
		virtual ~RequestState(void);
		virtual RequestState	*process(Request &request, Client &client) = 0;
};

#endif
