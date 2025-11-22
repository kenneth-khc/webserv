#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <ctime>
#include <cstdio>

void	runTimer(int fd) {
	char		buffer[1024];
	std::time_t	timer;
	bool		nextSend = false;
	bool		startTimer = false;

	while (ssize_t bytes = recv(fd, buffer, 1023, 0)) {
		if (startTimer == false && bytes == -1) {
			timer = std::time(0);
			startTimer = true;
		}
		if (bytes > 0)
			buffer[bytes] = '\0';
	}
	std::cout << "Waited for " << std::time(0) - timer << " seconds" << '\n';
}

int	runTest(int fd, const std::string &testString, ssize_t length) {
	std::cout << "Test string: ";
	for (std::string::const_iterator it = testString.begin(); it != testString.end(); it++) {
		if (*it == '\r')
			std::cout << "\\r";
		else if (*it == '\n')
			std::cout << "\\n";
		else
			std::cout << *it;
	}
	std::cout << '\n';

	if (length == -1)
		length = testString.size();

	if (send(fd, testString.c_str(), length, 0) == -1) {
		std::perror("send() failed");
		return (-1);
	}
	runTimer(fd);
	return (0);
}

int	main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Exec arguments: [IP Address] [Port]" << '\n';
		return (1);
	}

	struct addrinfo	hints = {};
	struct addrinfo	*result = 0;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int	code;

	code = getaddrinfo(argv[1], argv[2], &hints, &result);

	if (!code) {
		std::cerr << "getaddrinfo() failed: " << gai_strerror(code) << '\n';
		return (code);
	}

	int	fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	fcntl(fd, F_SETFL, O_NONBLOCK);

	if (connect(fd, result->ai_addr, result->ai_addrlen) == -1) {
		std::perror("connect() failed");
		return (-1);
	}

	std::string	testString;

	std::cout << "Testing Keep-Alive timeout:" << '\n';
	testString = "GET / HTTP/1.1\r\nHost: a\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}
	std::cout << '\n';

	std::cout << "Testing Client Header timeout:" << '\n';
	testString = "GET / HTTP/1.1\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}
	std::cout << '\n';

	std::cout << "Testing Client Body timeout:" << '\n';
	testString = "GET / HTTP/1.1\r\nHost: a\r\nTransfer-Encoding: chunked\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}

	testString = "GET / HTTP/1.1\r\nHost: a\r\nContent-Length: 1\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}
	std::cout << '\n';

	std::cout << "Testing imaginary path:" << '\n';
	testString = "GET /a HTTP/1.1\r\nHost: a\r\nContent-Length: 1\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}

	testString = "GET /a HTTP/1.1\r\nHost: a\r\nTransfer-Encoding: chunked\r\n\r\n?";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}
	std::cout << '\n';

	std::cout << "Testing unallowed method:" << '\n';
	testString = "DEL / HTTP/1.1\r\nHost: a\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}

	testString = "DEL / HTTP/1.1\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}

	testString = "DEL / HTTP/1.1\r\nHost: a\r\nTransfer-Encoding: chunked\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}

	testString = "DEL /a HTTP/1.1\r\nHost: a\r\nTransfer-Encoding: chunked\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}

	testString = "DEL /a HTTP/1.1\r\nHost: a\r\nContent-Length: 1\r\n\r\n";
	if (runTest(fd, testString, -1) == -1) {
		return (-1);
	}
	std::cout << std::endl;
	return (0);
}