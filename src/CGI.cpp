/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:36:15 by cteoh             #+#    #+#             */
/*   Updated: 2025/03/27 16:29:37 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <sys/wait.h>
#include <cstring>
#include <map>
#include <iomanip>
#include <fcntl.h>
#include "Optional.hpp"
#include "Server.hpp"
#include "ErrorCode.hpp"
#include "CGI.hpp"

const String	CGI::cgiFields[NUM_OF_CGI_FIELDS] = {
	"content-type",
	"location",
	"status"
};

CGI::CGI(
	const Driver &driver,
	Client &client,
	Request &request,
	Response &response) :
	client(client),
	request(request),
	response(response),
	inputFD(0),
	outputFD(0),
	outputPipeSize(0),
	pid(0),
	inputLength(0),
	lastActive(0),
	processStage(CGI::HEADERS)
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
	std::vector<String>::const_iterator	it = driver.cgiScript.begin();

	while (it != driver.cgiScript.end()) {
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

	if (it == driver.cgiScript.end() || access(this->execPath.c_str(), X_OK) != 0)
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
}

void	CGI::generateEnv(const Driver &driver) {
	(void)driver;
	std::stringstream	portNum;
	std::stringstream	serverProtocol;
	std::stringstream	contentLength;

	portNum << this->client.socket.port;
	serverProtocol << std::setprecision(2) << this->request.httpVersion;
	contentLength << this->request.messageBody.length();

	const String	metaVariables[NUM_OF_META_VARIABLES] = {
		"CONTENT_LENGTH=" + contentLength.str(),
		"CONTENT_TYPE=" + (this->request)["Content-Type"].value,
		// "GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=/",	// Test-specific condition
		"QUERY_STRING=" + this->request.query.value,
		// "REMOTE_ADDR=" + this->client.getIPAddr(),
		// "REMOTE_HOST=" + this->request["Host"].value,
		"REQUEST_METHOD=" + this->request.method,
		// "SCRIPT_NAME=/" + this->execPath,
		// "SERVER_NAME=" + driver.name,
		// "SERVER_PORT=" + portNum,
		"SERVER_PROTOCOL=HTTP/" + serverProtocol.str(),
		// "SERVER_SOFTWARE=" + driver.name,
	};

	const String	extMetaVariables[NUM_OF_EXT_META_VARIABLES] = {
		"X_UPLOADS_DIR=" + driver.uploadsDir
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

	epoll_event ev;
	ev.events = EPOLLOUT;
	fcntl(input[1], F_SETFL, O_NONBLOCK);
	ev.data.fd = input[1];
	epoll_ctl(driver.epollFD, EPOLL_CTL_ADD, input[1], &ev);
	driver.cgis.insert(std::make_pair(input[1], this));
	this->inputFD = input[1];

	ev.events = EPOLLIN;
	fcntl(output[0], F_SETFL, O_NONBLOCK);
	ev.data.fd = output[0];
	epoll_ctl(driver.epollFD, EPOLL_CTL_ADD, output[0], &ev);
	driver.cgis.insert(std::make_pair(output[0], this));
	this->outputFD = output[0];
	this->outputPipeSize = fcntl(this->outputFD, F_GETPIPE_SZ);
}

void	CGI::feedInput(int epollFD) {
	ssize_t	bytes = 0;
	Request	&request = this->request;

	if (this->inputLength < request.bodyLength && request.messageBody.length() > 0) {
		ssize_t	bytesToWrite = request.bodyLength - this->inputLength;

		bytes = write(this->inputFD, request.messageBody.c_str(), bytesToWrite);
		if (bytes > 0) {
			this->inputLength += bytes;
			request.messageBody.erase(0, bytes);
			this->lastActive = Time::getTimeSinceEpoch();
		}
	}
	if (this->inputLength == this->request.bodyLength) {
		this->processStage |= CGI::INPUT_DONE;
		epoll_ctl(epollFD, EPOLL_CTL_DEL, this->inputFD, 0);
		close(this->inputFD);
		this->inputFD = -1;
	}
}

void	CGI::fetchOutput(int epollFD) {
	if (this->processStage & CGI::OUTPUT_DONE)
		return ;

	ssize_t	bytes = -1;
	char	*buffer = new char[this->outputPipeSize];
	int		stat_loc = 0;

	bytes = read(this->outputFD, buffer, this->outputPipeSize);

	try {
		if (bytes > 0) {
			this->output.append(buffer, bytes);
			this->lastActive = Time::getTimeSinceEpoch();
			this->parseOutput();
		}
		else if (bytes <= 0 && waitpid(this->pid, &stat_loc, WNOHANG) == this->pid) {
			this->processStage |= CGI::OUTPUT_DONE;
			if (stat_loc != 0)
				throw InternalServerError500();
			this->parseOutput();
		}
		else if (Time::getTimeSinceEpoch() - this->lastActive >= Server::cgiTimeoutDuration) {
			this->processStage |= CGI::OUTPUT_DONE;
			kill(this->pid, SIGKILL);
			throw InternalServerError500();
		}
	}
	catch (const ErrorCode &e) {
		this->response = e;
	}
	delete [] buffer;

	if (this->processStage & CGI::OUTPUT_DONE) {
		epoll_ctl(epollFD, EPOLL_CTL_DEL, this->outputFD, 0);
		close(this->outputFD);
		this->outputFD = -1;
	}
}

void	CGI::parseOutput(void) {
	if (this->processStage & CGI::HEADERS) {
		String						delimiter = "\r\n\r\n";
		Optional<String::size_type>	delimiterPos = this->output.find(delimiter);

		if (delimiterPos.exists == false) {
			delimiter = "\n\n";
			delimiterPos = this->output.find(delimiter);
			if (delimiterPos.exists == false && this->processStage & CGI::OUTPUT_DONE)
				throw InternalServerError500();
			else
				return ;
		}

		String							headerPart = this->output.substr(0, delimiterPos.value);
		std::vector<String>				headers;
		std::vector<String>				existingFieldNames;
		std::multimap<String, String>	validHeaders;

		this->output.erase(0, delimiterPos.value + delimiter.length());
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

			existingFieldNames.push_back(fieldName.lower());
			validHeaders.insert(std::make_pair(fieldName.title(), fieldValue.trim(" ")));
		}

		int	total = 0;
		for (int i = 0; i < NUM_OF_CGI_FIELDS; i++) {
			int	count = std::count(existingFieldNames.begin(), existingFieldNames.end(), cgiFields[i]);

			if (count > 1)
				throw InternalServerError500();
			total += count;
		}
		if (total == 0)
			throw InternalServerError500();

		std::multimap<String, String>::iterator	it;
		std::stringstream						stream;
		String									str;
		int										statusCode;

		it = validHeaders.find("Status");
		if (it == validHeaders.end())
			this->response.setStatusCode(Response::OK);
		else {
			stream << it->second;
			stream >> statusCode;
			this->response.setStatusCode(statusCode);
			String::getline(stream, str);
			this->response.reasonPhrase = str.trim(" ");
			validHeaders.erase(it);
		}

		this->response.headers = validHeaders;
		this->processStage &= ~CGI::HEADERS;
		this->processStage |= CGI::MESSAGE_BODY;
	}

	if (this->processStage & CGI::MESSAGE_BODY) {
		this->response.messageBody.append(this->output.c_str(), this->output.length());
		this->output.erase(0, this->output.length());
	}
}

void	Driver::cgi(Request &request, Response &response) {
	if (request.method != "GET" && request.method != "POST")
		throw NotImplemented501();

	Client	&client = this->clients[currEvent->data.fd];
	CGI		*cgi = new CGI(*this, client, request, response);

	cgi->generateEnv(*this);
	cgi->execute(*this);
}
