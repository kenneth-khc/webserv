/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientBodyTimer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:15:36 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:57:38 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTBODYTIMER_HPP
# define CLIENTBODYTIMER_HPP

# include "Timer.hpp"

class ClientBodyTimer : public Timer {
	public:
		ClientBodyTimer(void);
		~ClientBodyTimer(void);
		ClientBodyTimer(const ClientBodyTimer &obj);
		ClientBodyTimer	&operator=(const ClientBodyTimer &obj);

		void	update(void);
		bool	isTimeout(void) const;
		int		getType(void) const;
		int		getLogType(void) const;
		Timer	*clone(void) const;
};

#endif
