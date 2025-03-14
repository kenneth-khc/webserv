/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:36:15 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/08 16:37:55 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <sstream>
#include <sys/wait.h>
#include <cstring>
#include <map>
#include <iomanip>
#include "Optional.hpp"
#include "Time.hpp"
#include "ErrorCode.hpp"
#include "CGI.hpp"
#include "Driver.hpp"

CGI::CGI(const Driver &driver, const Request &request) :
	driver(driver),
	request(request)
{
	Optional<String::size_type>	extPos = request.absolutePath.find(".");

	if (extPos.exists == false)
		throw NotFound404();

	Optional<String::size_type>	pathInfoPos = request.absolutePath.find("/", extPos.value);

	if (pathInfoPos.exists == true)
		this->extension = request.absolutePath.substr(extPos.value, pathInfoPos.value - extPos.value);
	else
		this->extension = request.absolutePath.substr(extPos.value);

	String								pathInfo;
	std::vector<String>::const_iterator	it = driver.cgiScript.begin();

	while (it != driver.cgiScript.end()) {
		if (this->extension == ("." + *it)) {
			if (pathInfoPos.exists == true) {
				this->execPath = request.absolutePath.substr(1, pathInfoPos.value - 1);
				pathInfo = request.absolutePath.substr(pathInfoPos.value);
			}
			else
				this->execPath = request.absolutePath.substr(1);
			break ;
		}
		it++;
	}

	if (it == driver.cgiScript.end() || access(this->execPath.c_str(), X_OK) != 0)
		throw NotFound404();

	this->generateEnv();

	this->argv = new char*[2]();
	this->argv[1] = new char[request.absolutePath.length() + 1];
	std::memcpy(this->argv[1], request.absolutePath.c_str(), request.absolutePath.length());
	this->argv[1][request.absolutePath.length()] = '\0';
}

CGI::~CGI(void) {
	for (std::vector<char *>::const_iterator it = this->envp.begin(); it != this->envp.end(); it++)
		delete [] *it;
	delete this->argv[0];
	delete [] this->argv;
}

void	CGI::generateEnv(void)
{
	std::stringstream	stream;
	String				portNumStr;
	String				serverProtocolStr;
	const Client&		client = driver.clients.find(driver.readyEvents[0].data.fd)->second;
	String				contentLength = request["Content-Length"].value;

	stream << client.socket.port;
	portNumStr = stream.str();
	stream.str("");

	stream << std::setprecision(2) << request.httpVersion;
	serverProtocolStr = stream.str();

	if (contentLength == "0")
		contentLength = "";

	const String	metaVariables[NUM_OF_META_VARIABLES] = {
		"CONTENT_LENGTH=" + contentLength,
		"CONTENT_TYPE=" + request["Content-Type"].value,
		"GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=" + pathInfo,
		"QUERY_STRING=" + request.query.value,
		"REMOTE_ADDR=" + client.getIPAddr(),
		"REMOTE_HOST=" + request["Host"].value,
		"REQUEST_METHOD=" + request.method,
		"SCRIPT_NAME=" + this->execPath,
		"SERVER_NAME=" + driver.name,
		"SERVER_PORT=" + portNumStr,
		"SERVER_PROTOCOL=HTTP/" + serverProtocolStr,
		"SERVER_SOFTWARE=" + driver.name,
		"X_SID=" + request.cookies.find("sid")->second.value,
		"X_UPLOADS_DIR=" + driver.uploadsDir
	};

	int	i = -1;
	while (environ[++i] != 0) {
		String	temp(environ[i]);
		this->envp.push_back(new char[temp.length() + 1]);
		std::memcpy(this->envp[i], environ[i], temp.length());
		this->envp[i][temp.length()] = '\0';
	}
	for (int j = 0; j < NUM_OF_META_VARIABLES; j++) {
		this->envp.push_back(new char[metaVariables[j].length() + 1]);
		std::memcpy(this->envp[i], metaVariables[j].c_str(), metaVariables[j].length());
		this->envp[i][metaVariables[j].length()] = '\0';
		i++;
	}
	if (extension == ".php") {
		const String	phpMetaVariables[PHP_META_VARIABLES] = {
			"REDIRECT_STATUS=200",
			"SCRIPT_FILENAME=" + this->execPath
		};

		for (int j = 0; j < PHP_META_VARIABLES; j++) {
			this->envp.push_back(new char[phpMetaVariables[j].length() + 1]);
			std::memcpy(this->envp[i], phpMetaVariables[j].c_str(), phpMetaVariables[j].length());
			this->envp[i][phpMetaVariables[j].length()] = '\0';
			i++;
		}
	}
	this->envp.push_back(0);
}

void	CGI::execute(void) {
	const String	&contentLength = request["Content-Length"].value;

	socketpair(AF_UNIX, SOCK_STREAM, 0, this->dataSend);
	socketpair(AF_UNIX, SOCK_STREAM, 0, this->dataRecv);
	this->pid = fork();
	if (this->pid == 0) {
		close(this->dataSend[1]);
		dup2(this->dataSend[0], STDIN_FILENO);
		close(this->dataSend[0]);
		close(this->dataRecv[0]);
		dup2(this->dataRecv[1], STDOUT_FILENO);
		close(this->dataRecv[1]);
		execve(this->execPath.c_str(), argv, this->envp.data());
		std::exit(1);
	}

	if (contentLength != "" && contentLength != "0") {
		(void)!write(this->dataSend[1], request.messageBody.c_str(), request.find< Optional<String::size_type> >("Content-Length").value);
	}
	close(this->dataSend[0]);
	close(this->dataSend[1]);
	close(this->dataRecv[1]);

	std::time_t	startTime = Time::getTimeSinceEpoch();
	std::time_t	currTime;
	char		buffer[1024];
	bool		isEmpty = false;
	ssize_t		bytes = 0;

	while (true) {
		bytes = recv(this->dataRecv[0], buffer, 1023, MSG_DONTWAIT);
		currTime = Time::getTimeSinceEpoch();
		if (bytes <= 0)
			isEmpty = true;
		else {
			startTime = currTime;
			isEmpty = false;
			buffer[bytes] = '\0';
			this->output += buffer;
		}
		if (bytes <= 0 && waitpid(this->pid, NULL, WNOHANG) == this->pid)
			break ;
		else if (isEmpty == true && currTime - startTime > CGI_TIMEOUT_VALUE) {
			kill(this->pid, SIGKILL);
			throw InternalServerError500();
		}
	};
	close(this->dataRecv[0]);
}

void	CGI::parseOutput(Response &response) const {
	String						delimiter = "\r\n\r\n";
	Optional<String::size_type>	delimiterPos = this->output.find(delimiter);

	if (delimiterPos.exists == false) {
		delimiter = "\n\n";
		delimiterPos = this->output.find(delimiter);
		if (delimiterPos.exists == false)
			throw InternalServerError500();
	}

	String						headerPart = this->output.substr(0, delimiterPos.value);
	String						bodyPart = this->output.substr(delimiterPos.value + delimiter.length());
	std::vector<String>			headers;
	std::map<String, String>	headerKeyValues;

	if (delimiter == "\r\n\r\n")
		headers = headerPart.split("\r\n");
	else
		headers = headerPart.split("\n");

	for (std::vector<String>::const_iterator it = headers.begin(); it != headers.end(); it++) {
		Optional<String::size_type>	pos = it->find(":");

		if (pos.exists == false)
			throw InternalServerError500();

		String	fieldName = it->substr(0, pos.value);
		String	fieldValue = it->substr(pos.value + 1);

		if (fieldValue.length() == 0)
			continue ;

		String	trim = fieldName.trim(" ");

		if (trim.length() != fieldName.length())
			throw InternalServerError500();
		if (headerKeyValues.find(fieldName) != headerKeyValues.end())
			throw InternalServerError500();

		headerKeyValues.insert(std::make_pair(fieldName, fieldValue));
	}
	if (headerKeyValues.size() == 0)
		throw InternalServerError500();

	std::map<String, String>::const_iterator	it;
	std::stringstream							stream;
	String										str;
	int											statusCode;

	it = headerKeyValues.find("Status");
	if (it == headerKeyValues.end())
		response.setStatusCode(Response::OK);
	else {
		stream << it->second;
		stream >> statusCode;
		response.setStatusCode(statusCode);
		String::getline(stream, str);
		response.reasonPhrase = str.trim(" ");
	}

	for (it = headerKeyValues.begin(); it != headerKeyValues.end(); it++)
		response.insert(it->first, it->second);
}

void	Driver::cgi(Response &response, const Request &request) const {
	CGI	cgi(*this, request);

	cgi.execute();
	cgi.parseOutput(response);
}
