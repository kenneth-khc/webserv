/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:45:52 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/24 13:01:52 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include "String.hpp"
# include "Request.hpp"
# include "CGIInput.hpp"
# include "CGIOutput.hpp"
# include "CGITimer.hpp"
# include "CGIScriptBlock.hpp"

# define NUM_OF_CGI_FIELDS 3
# define NUM_OF_META_VARIABLES 13
# define PHP_META_VARIABLES 3

struct Driver;
class Client;
struct Server;

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
		String				scriptName;
		String				pathInfo;
		std::vector<char *>	envp;
		std::vector<char *>	argv;

		pid_t				pid;
		CGIInput			*input;
		CGIOutput			*output;
		CGITimer			*timer;

		CGI(Client &client, Request &request, Response &response,
				String &extension, String &pathInfo, String &scriptName,
					std::vector<CGIScriptBlock>::const_iterator &block);
		~CGI(void);
		void	generateEnv(const String &webServerName);
		void	execute(int epollFD, std::map<int, CGI*> &cgis);
};

#endif
