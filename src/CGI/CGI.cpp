/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:36:15 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/21 07:36:14 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <map>
#include <iomanip>
#include <fcntl.h>
#include <signal.h>
#include "Optional.hpp"
#include "Driver.hpp"
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "connection.hpp"
#include "CGI.hpp"

const String	CGI::cgiFields[NUM_OF_CGI_FIELDS] = {
	"content-type",
	"location",
	"status"
};

/*
	Converts the received request data into CGI meta variables.
*/
CGI::CGI(Client &client, Request &request, Response &response) :
	client(client),
	request(request),
	response(response),
	pid(0),
	input(0),
	output(0),
	timer(0)
{
	Optional<String::size_type>	extPos = request.path.find(".");

	if (extPos.exists == false)
		throw NotFound404();

	Optional<String::size_type>	pathInfoPos = request.path.find("/", extPos.value);

	if (pathInfoPos.exists == true)
		this->extension = request.path.substr(extPos.value, pathInfoPos.value - extPos.value);
	else
		this->extension = request.path.substr(extPos.value);

	String								pathInfo;
	std::set<String>::const_iterator	it = request.location->cgiScripts.begin();

	while (it != request.location->cgiScripts.end()) {
		if (this->extension == ("." + *it)) {
			if (pathInfoPos.exists == true) {
				this->scriptName = request.path.substr(1, pathInfoPos.value - 1);
				pathInfo = request.path.substr(pathInfoPos.value);
			}
			else
				this->scriptName = request.path.substr(1);
			break ;
		}
		it++;
	}
	this->execPath = this->scriptName;

	if (it == request.location->cgiScripts.end() || access(this->execPath.c_str(), X_OK) != 0)
		throw NotFound404();

	String						cgiName;
	Optional<String::size_type>	cgiNamePos = this->execPath.find_last_of("/");

	if (cgiNamePos.exists == true)
		cgiName = this->execPath.substr(cgiNamePos.value + 1);
	else
		cgiName = this->execPath;

	this->argv.push_back(new char[cgiName.length() + 1]);
	std::memcpy(this->argv[0], cgiName.c_str(), cgiName.length() + 1);
	this->argv.push_back(0);
}

CGI::~CGI(void) {
	kill(this->pid, SIGKILL);
	for (std::vector<char *>::const_iterator it = this->envp.begin(); it != this->envp.end(); it++) {
		if (*it != 0)
			delete [] *it;
	}
	for (std::vector<char *>::const_iterator it = this->argv.begin(); it != this->argv.end(); it++) {
		if (*it != 0)
			delete [] *it;
	}
	if (this->input != 0)
		delete this->input;
	if (this->output != 0)
		delete this->output;
	if (this->timer != 0)
		delete this->timer;
	std::remove(this->client.cgis.begin(), this->client.cgis.end(), this);
}

/*
	Creates the entire list of meta variables needed for CGI execution.
*/
void	CGI::generateEnv(const String &webServerName) {
	String				host = this->request["Host"].value;
	std::stringstream	serverPort;
	std::stringstream	contentLength;

	serverPort << this->client.socket->port;
	contentLength << this->request.messageBody.length();

	const String	metaVariables[NUM_OF_META_VARIABLES] = {
		"CONTENT_LENGTH=" + contentLength.str(),
		"CONTENT_TYPE=" + (this->request)["Content-Type"].value,
		"GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=" + this->request.path,
		"QUERY_STRING=" + this->request.query.value,
		"REMOTE_ADDR=" + this->client.socket->ip,
		"REMOTE_HOST=" + String(host).consumeUntil(":").value_or(host),
		"REQUEST_METHOD=" + this->request.method,
		"SCRIPT_NAME=",
		"SERVER_NAME=" + String(host).consumeUntil(":").value_or(host),
		"SERVER_PORT=" + serverPort.str(),
		"SERVER_PROTOCOL=HTTP/" + this->request.httpVersion,
		"SERVER_SOFTWARE=" + webServerName,
	};

	const String	extMetaVariables[NUM_OF_EXT_META_VARIABLES] = {
		"X_UPLOADS_DIR=" + this->request.location->uploadDirectory
	};

	int 		i = 0;
	std::size_t	len = 0;
	for (int j = 0; j < NUM_OF_META_VARIABLES; j++) {
		len = metaVariables[j].length() + 1;

		this->envp.push_back(new char[len]);
		std::memcpy(this->envp[i], metaVariables[j].c_str(), len);
		i++;
	}

	for (int j = 0; j < NUM_OF_EXT_META_VARIABLES; j++) {
		len = extMetaVariables[j].length() + 1;

		this->envp.push_back(new char[len]);
		std::memcpy(this->envp[i], extMetaVariables[j].c_str(), len);
		i++;
	}

	if (extension == ".php") {
		const String	phpMetaVariables[PHP_META_VARIABLES] = {
			"REDIRECT_STATUS=200",
			"SCRIPT_FILENAME=" + this->execPath,
			"DOCUMENT_ROOT=" + this->request.location->root
		};

		for (int j = 0; j < PHP_META_VARIABLES; j++) {
			len = phpMetaVariables[j].length() + 1;

			this->envp.push_back(new char[len]);
			std::memcpy(this->envp[i], phpMetaVariables[j].c_str(), len);
			i++;
		}
	}

	std::multimap<String, String>::const_iterator it = this->request.headers.begin();
	while (it != this->request.headers.end()) {
		String	protocolMetaVariable = "HTTP_";

		for (String::size_type i = 0; i < it->first.length(); i++) {
			if (it->first[i] == '-')
				protocolMetaVariable.push_back('_');
			else
				protocolMetaVariable.push_back(std::toupper(it->first[i]));
		}
		protocolMetaVariable += "=" + it->second;

		len = protocolMetaVariable.length() + 1;
		this->envp.push_back(new char[len]);
		std::memcpy(this->envp[i], protocolMetaVariable.c_str(), len);
		i++;
		it++;
	}
	this->envp.push_back(0);
}

/*
	Creates two pipes, one for sending data into CGI and another for receiving
	results from it.

	The write end of the input pipe, and the read end of the output pipe are
	added to epoll to allow the server to process other requests during CGI
	execution.
*/
void	CGI::execute(int epollFD, std::map<int, CGI*> &cgis) {
	int	input[2];
	int	output[2];

	(void)!pipe(input);
	(void)!pipe(output);

	this->pid = fork();
	if (this->pid == 0) {
		close(input[1]);
		dup2(input[0], STDIN_FILENO);
		close(input[0]);
		close(output[0]);
		dup2(output[1], STDOUT_FILENO);
		close(output[1]);
		execve(this->execPath.c_str(), this->argv.data(), this->envp.data());
		std::exit(1);
	}

	close(input[0]);
	close(output[1]);

	this->input = new CGIInput(*this, epollFD, cgis, input[1]);
	cgis.insert(std::make_pair(input[1], this));

	this->output = new CGIOutput(*this, epollFD, cgis, output[0]);
	cgis.insert(std::make_pair(output[0], this));

	this->timer = new CGITimer();
}
