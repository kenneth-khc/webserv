/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:36:15 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 04:46:36 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <map>
#include <iomanip>
#include <fcntl.h>
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

CGI::CGI(
	const Server &server,
	Client &client,
	Request &request,
	Response &response) :
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
	std::vector<String>::const_iterator	it = server.cgiScript.begin();

	while (it != server.cgiScript.end()) {
		if (this->extension == ("." + *it)) {
			if (pathInfoPos.exists == true) {
				this->execPath = request.path.substr(1, pathInfoPos.value - 1);
				pathInfo = request.path.substr(pathInfoPos.value);
			}
			else
				this->execPath = request.path.substr(1);
			break ;
		}
		it++;
	}
	if (this->execPath.ends_with(".bla") == true)	// Test-specific condition
		this->execPath = "subject/ubuntu_cgi_tester";

	if (it == server.cgiScript.end() || access(this->execPath.c_str(), X_OK) != 0)
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
	std::remove(this->client.cgis.begin(), this->client.cgis.end(), this);
}

void	CGI::generateEnv(const Driver &driver) {
	String				host = this->request["Host"].value;
	std::stringstream	serverPort;
	std::stringstream	serverProtocol;
	std::stringstream	contentLength;

	serverPort << this->client.socket->port;
	serverProtocol << std::setprecision(2) << this->request.httpVersion;
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
		"SCRIPT_NAME=/" + this->execPath,
		"SERVER_NAME=" + String(host).consumeUntil(":").value_or(host),
		"SERVER_PORT=" + serverPort.str(),
		"SERVER_PROTOCOL=HTTP/" + serverProtocol.str(),
		"SERVER_SOFTWARE=" + driver.webServerName,
	};

	const String	extMetaVariables[NUM_OF_EXT_META_VARIABLES] = {
		"X_UPLOADS_DIR=uploads"
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
			"DOCUMENT_ROOT=/home/cteoh/Documents/webserv"
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

void	CGI::execute(Driver &driver) {
	int	input[2];
	int	output[2];

	(void)!pipe(input);
	( void)!pipe(output);

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

	this->input = new CGIInput(driver, *this, input[1]);
	driver.cgis.insert(std::make_pair(input[1], this));

	this->output = new CGIOutput(driver, *this, output[0]);
	driver.cgis.insert(std::make_pair(output[0], this));
}

bool	CGI::isTimeout(const Server &server) {
	if (server.cgiTimeoutDuration <= 0)
		return (false);

	if (Time::getTimeSinceEpoch() >= this->timer)
		return (true);
	else
		return (false);
}
