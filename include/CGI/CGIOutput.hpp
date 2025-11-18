/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIOutput.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:33:38 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 17:23:47 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIOUTPUT_HPP
# define CGIOUTPUT_HPP

# include <set>
# include <map>
# include "Timer.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "CGIOutputState.hpp"

class CGI;

class CGIOutput {
		void	processState(const ssize_t &bytes);
	public:
		CGI					&cgi;
		const int			epollFD;
		std::map<int, CGI*>	&cgis;
		int					fd;
		const int			pipeSize;
		String				output;
		CGIOutputState		*state;

		CGIOutput(Driver &driver, CGI &cgi, int fd);
		~CGIOutput(void);

		void	fetch(std::set<Timer*>& activeTimers);
		void	close(void);
};

#endif
