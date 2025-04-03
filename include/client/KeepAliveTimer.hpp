/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeepAliveTimer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:57:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 17:08:22 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEEPALIVETIMER_HPP
# define KEEPALIVETIMER_HPP

# include "Timer.hpp"

class KeepAliveTimer : public Timer {
	public:
		KeepAliveTimer(void);
		~KeepAliveTimer(void);
		KeepAliveTimer(const KeepAliveTimer &obj);
		KeepAliveTimer	&operator=(const KeepAliveTimer &obj);

		void	update(void);
		bool	isTimeout(void) const;
		int		getType(void) const;
		int		getLogType(void) const;
		Timer	*clone(void) const;
};

#endif
