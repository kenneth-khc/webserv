/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIOutputState.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:56:17 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 03:57:35 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIOUTPUTSTATE_HPP
# define CGIOUTPUTSTATE_HPP

# include <sys/types.h>
# include "String.hpp"
# include "Response.hpp"

class CGIOutputState {
	public:
		virtual ~CGIOutputState(void);
		virtual CGIOutputState	*parse(Response &response, String &output, ssize_t bytes) = 0;
};

#endif
