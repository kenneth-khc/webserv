/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeepAliveState.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:57:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 01:10:43 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEEPALIVESTATE_HPP
# define KEEPALIVESTATE_HPP

# include "ClientTimerState.hpp"

class KeepAliveState : public ClientTimerState {
	public:
		KeepAliveState(void);
		~KeepAliveState(void);
		KeepAliveState(const KeepAliveState &obj);
		KeepAliveState	&operator=(const KeepAliveState &obj);

		void				update(const Server &server);
		bool				isTimeout(const Server &server) const;
		int					getState(void) const;
		int					getLogState(void) const;
		ClientTimerState	*clone(void) const;
};

#endif
