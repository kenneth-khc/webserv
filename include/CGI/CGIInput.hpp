/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInput.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:11:47 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 03:46:56 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIINPUT_HPP
# define CGIINPUT_HPP

# include <map>
# include <set>
# include "String.hpp"
# include "Request.hpp"

class Driver;
class CGI;

class CGIInput {
		CGIInput(void);
	public:
		CGI						&cgi;
		const int				epollFD;
		std::map<int, CGI*>		&cgis;
		int						fd;
		String::size_type		inputLength;
		const String::size_type	contentLength;

		CGIInput(Driver &driver, CGI &cgi, int fd);
		~CGIInput(void);

		void	feed(std::set<CGI*>& activeCGIs);
		void	close(void);
};

#endif
