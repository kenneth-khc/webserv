/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:45:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/09 10:10:52 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <unistd.h>
# include "String.hpp"
# include "Request.hpp"
# include "Driver.hpp"

# define NUM_OF_META_VARIABLES 15
# define PHP_META_VARIABLES 2
# define CGI_TIMEOUT_VALUE 3

class CGI {
		CGI(void);
		CGI(const CGI &obj);
		CGI	&operator=(const CGI &obj);
	public:
		const Driver		&driver;
		const Request		&request;
		String				execPath;
		String				extension;
		String				pathInfo;
		std::vector<char *>	envp;
		char				**argv;
		int					input[2];
		int					output[2];
		pid_t				pid;
		String				response;
		String				inputContentLength;

		CGI(const Driver &driver, const Request &request);
		~CGI(void);
		void	generateEnv(void);
		void	execute(void);
		void	parseOutput(Response &response) const;
};

#endif
