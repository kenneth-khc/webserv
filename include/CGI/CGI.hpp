/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:45:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 04:44:20 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include "Time.hpp"
# include "String.hpp"
# include "Request.hpp"
# include "CGIInput.hpp"
# include "CGIOutput.hpp"

# define NUM_OF_CGI_FIELDS 3
# define NUM_OF_META_VARIABLES 13
# define NUM_OF_EXT_META_VARIABLES 1
# define PHP_META_VARIABLES 3

class Driver;
class Client;
class Server;

class CGI {
		CGI(void);
		CGI(const CGI &obj);
		CGI	&operator=(const CGI &obj);
	public:
		static const String	cgiFields[NUM_OF_CGI_FIELDS];

		Client				&client;
		Request				&request;
		Response			&response;

		String				execPath;
		String				extension;
		String				pathInfo;
		std::vector<char *>	envp;
		std::vector<char *>	argv;

		pid_t				pid;
		CGIInput			*input;
		CGIOutput			*output;
		std::time_t			timer;

		CGI(const Server &server, Client &client, Request &request, Response &response);
		~CGI(void);
		void	generateEnv(const Driver &driver);
		void	execute(Driver &driver);
		bool	isTimeout(const Server &server);
};

#endif
