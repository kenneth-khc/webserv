/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIMessageBodyState.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:01:49 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/02 22:48:03 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMESSAGEBODYSTATE_HPP
# define CGIMESSAGEBODYSTATE_HPP

# include "CGIOutputState.hpp"

class CGIMessageBodyState : public CGIOutputState {
	public:
		CGIOutputState	*parse(Response &response, String &output, ssize_t bytes);
};

#endif
