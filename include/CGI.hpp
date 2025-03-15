/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:45:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/08 16:19:33 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <unistd.h>
# include "String.hpp"
# include "Request.hpp"
# include "Driver.hpp"
# include "Client.hpp"

# define NUM_OF_META_VARIABLES 15
# define PHP_META_VARIABLES 2
# define CGI_TIMEOUT_VALUE 3

class CGI {
		CGI(void);
		CGI(const CGI &obj);
		CGI	&operator=(const CGI &obj);
	public:
		const Server		&server;
		const Request		&request;
		String				execPath;
		String				extension;
		String				pathInfo;
		std::vector<char *>	envp;
		char				**argv;
		int					dataSend[2];
		int					dataRecv[2];
		pid_t				pid;
		String				output;

		CGI(const Server &server, const Request &request);
		~CGI(void);
		void	generateEnv(const Client*);
		void	execute(void);
		void	parseOutput(Response &response) const;
};

#endif
