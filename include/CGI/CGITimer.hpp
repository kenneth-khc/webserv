/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGITimer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:10:24 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 17:15:47 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGITIMER_HPP
# define CGITIMER_HPP

# include "Timer.hpp"

class CGITimer : public Timer {
		int		getLogType(void) const;
	public:
		CGITimer(void);
		~CGITimer(void);
		CGITimer(const CGITimer &obj);
		CGITimer	&operator=(const CGITimer &obj);

		void	update(void);
		bool	isTimeout(void) const;
		int		getType(void) const;
		Timer	*clone(void) const;
};

#endif
