/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientTimerState.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:48:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/01 00:56:24 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTTIMERSTATE_HPP
# define CLIENTTIMERSTATE_HPP

# include "Time.hpp"

class Server;

class ClientTimerState {
	protected:
		std::time_t		timeout;
	public:
		ClientTimerState(void);
		virtual ~ClientTimerState(void);
		ClientTimerState	&operator=(const ClientTimerState &obj);
		ClientTimerState(const ClientTimerState &obj);

		virtual void				update(const Server &server) = 0;
		virtual bool				isTimeout(const Server &server) const = 0;
		virtual	int					getState(void) const = 0;
		virtual int					getLogState(void) const = 0;
		virtual const std::time_t	&getTimeoutTime(void) const;
		virtual ClientTimerState	*clone(void) const = 0;

		enum State {
			KEEP_ALIVE,
			CLIENT_HEADER,
			CLIENT_BODY
		};
};

#endif
