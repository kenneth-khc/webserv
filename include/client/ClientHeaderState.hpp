/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHeaderState.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:07:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 01:10:10 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTHEADERSTATE_HPP
# define CLIENTHEADERSTATE_HPP

# include "ClientTimerState.hpp"

class ClientHeaderState : public ClientTimerState {
	public:
		ClientHeaderState(void);
		~ClientHeaderState(void);
		ClientHeaderState(const ClientHeaderState &obj);
		ClientHeaderState	&operator=(const ClientHeaderState &obj);

		void				update(const Server &server);
		bool				isTimeout(const Server &server) const;
		int					getState(void) const;
		int					getLogState(void) const;
		ClientTimerState	*clone(void) const;
};

#endif
