/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:41:19 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 17:15:29 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP
# define TIMER_HPP

# include "Time.hpp"

class Timer {
	protected:
		std::time_t		timeout;
	public:
		Timer(void);
		virtual ~Timer(void);
		Timer	&operator=(const Timer &obj);
		Timer(const Timer &obj);

		virtual void				update(void) = 0;
		virtual bool				isTimeout(void) const = 0;
		virtual const std::time_t	&getTimeoutTime(void) const;
		virtual int					getType(void) const = 0;
		virtual int					getLogType(void) const = 0;
		virtual Timer				*clone(void) const = 0;

		enum Type {
			KEEP_ALIVE,
			CLIENT_HEADER,
			CLIENT_BODY,
			CGI
		};
};

#endif
