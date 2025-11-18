/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBodyState.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:30:21 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/30 03:25:43 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEBODYSTATE_HPP
# define MESSAGEBODYSTATE_HPP

# include "RequestState.hpp"

class MessageBodyState : public RequestState {
		String::size_type	chunkSize;
		String::size_type	bodyLength;
		bool				hasChunkSize;
		bool				isLastChunk;
	public:
		MessageBodyState(void);

		RequestState	*process(Request &request, Client &client);
		int				getState(void) const;
};

#endif
