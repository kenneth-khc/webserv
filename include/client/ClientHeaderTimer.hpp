/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHeaderTimer.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:07:39 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:55:29 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENTHEADERTIMER_HPP
# define CLIENTHEADERTIMER_HPP

# include "Timer.hpp"

class ClientHeaderTimer : public Timer {
	public:
		ClientHeaderTimer(void);
		~ClientHeaderTimer(void);
		ClientHeaderTimer(const ClientHeaderTimer &obj);
		ClientHeaderTimer	&operator=(const ClientHeaderTimer &obj);

		void	update(void);
		bool	isTimeout(void) const;
		int		getType(void) const;
		int		getLogType(void) const;
		Timer	*clone(void) const;
};

#endif
