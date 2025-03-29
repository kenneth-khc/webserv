/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmptyState.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:17:34 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/29 22:24:39 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EMPTYSTATE_HPP
# define EMPTYSTATE_HPP

# include "RequestState.hpp"

class EmptyState : public RequestState {
	public:
		RequestState	*process(Request &request, Client &client);
};

#endif
