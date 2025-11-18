/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHeadersState.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:59:05 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/02 22:47:53 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHEADERSSTATE_HPP
# define CGIHEADERSSTATE_HPP

# include "CGI.hpp"
# include "CGIOutputState.hpp"

class CGIHeadersState : public CGIOutputState {
	public:
		CGIOutputState	*parse(Response &response, String &output, ssize_t bytes);
};

#endif
