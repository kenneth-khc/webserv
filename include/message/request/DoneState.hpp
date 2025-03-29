/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DoneState.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 02:33:05 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/30 03:29:58 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONESTATE_HPP
# define DONESTATE_HPP

# include "RequestState.hpp"

class DoneState : public RequestState {
	public:
		RequestState	*process(Request &request, Client &client);
		int				getState(void) const;
};

#endif
