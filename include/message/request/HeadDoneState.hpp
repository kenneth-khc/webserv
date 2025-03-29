/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadDoneState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:23:55 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/29 22:26:04 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADDONESTATE_HPP
# define HEADDONESTATE_HPP

# include "RequestState.hpp"

class HeadDoneState : public RequestState {
	public:
		RequestState	*process(Request &request, Client &client);
};

#endif
