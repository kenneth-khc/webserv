/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:45:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/26 00:50:10 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include "Time.hpp"
# include "String.hpp"
# include "Driver.hpp"
# include "Client.hpp"
# include "Request.hpp"

# define NUM_OF_CGI_FIELDS 3
# define NUM_OF_META_VARIABLES 6
# define NUM_OF_EXT_META_VARIABLES 1
# define PHP_META_VARIABLES 3

class CGI {
		static const String	cgiFields[NUM_OF_CGI_FIELDS];
		CGI(void);
		CGI(const CGI &obj);
		CGI	&operator=(const CGI &obj);
	public:
		Client				&client;
		Request				&request;
		Response			&response;

		String				execPath;
		String				extension;
		String				pathInfo;
		std::vector<char *>	envp;
		std::vector<char *>	argv;

		int					inputFD;
		int					outputFD;
		pid_t				pid;
		String::size_type	inputLength;
		String				output;
		std::time_t			lastActive;
		char				processStage;

		CGI(const Driver &driver, Client &client, Request &request, Response &response);
		~CGI(void);
		void	generateEnv(const Driver &driver);
		void	execute(Driver &driver);
		void	feedInput(int epollFD);
		void	fetchOutput(int epollFD);
		void	parseOutput(void);

		enum ProcessStage {
			INPUT_DONE = 0x001,
			HEADERS = 0x002,
			MESSAGE_BODY = 0x004,
			OUTPUT_DONE = 0x008
		};
};

#endif
