/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientBodyState.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:15:36 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 01:10:30 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTBODYSTATE_HPP
# define CLIENTBODYSTATE_HPP

# include "Server.hpp"
# include "ClientTimerState.hpp"

class ClientBodyState : public ClientTimerState {
	public:
		ClientBodyState(void);
		~ClientBodyState(void);
		ClientBodyState(const ClientBodyState &obj);
		ClientBodyState	&operator=(const ClientBodyState &obj);

		void				update(const Server &server);
		bool				isTimeout(const Server &server) const;
		int					getState(void) const;
		int					getLogState(void) const;
		ClientTimerState	*clone(void) const;
};

#endif
