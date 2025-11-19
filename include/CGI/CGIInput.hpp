/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInput.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:11:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/20 03:14:11 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIINPUT_HPP
# define CGIINPUT_HPP

# include <map>
# include <set>
# include "Timer.hpp"
# include "String.hpp"
# include "Request.hpp"

struct Driver;
class CGI;

class CGIInput {
		CGIInput(void);
	public:
		const int				epollFD;
		CGI						&cgi;
		std::map<int, CGI*>		&cgis;
		int						fd;
		String::size_type		inputLength;
		const String::size_type	contentLength;

		CGIInput(CGI &cgi, int epollFD, std::map<int, CGI*> &cgis, int fd);
		~CGIInput(void);

		void	feed(std::set<Timer*>& activeTimers);
		void	close(void);
};

#endif
