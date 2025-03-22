#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <ctime>
#include <stdio.h>

int	main(void) {
	struct addrinfo	hints = {};
	struct addrinfo	*result = 0;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	std::cout << "GET ADDR: " << getaddrinfo("127.0.0.1", "8000", &hints, &result) << std::endl;

	int	fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	std::cout << "FD: " << fd << std::endl;

	std::cout << "CONNECT: " << connect(fd, result->ai_addr, result->ai_addrlen) << std::endl;

	fcntl(fd, F_SETFL, O_NONBLOCK);

	std::cout << "SEND: " << send(fd, "GET / HTTP/1.1\r\nHost: a\r\n\r\nGET / HTTP/1.1\r\nHost: a\r\n\r\n", 53, 0) << std::endl;

	char		buffer[1024];
	std::time_t	timer;
	bool		nextSend = false;
	bool		startTimer = false;
	while (ssize_t bytes = recv(fd, buffer, 1023, 0)) {
		if (startTimer == false && bytes == -1) {
			timer = std::time(0);
			startTimer = true;
		}
		else if (nextSend == false && std::time(0) - timer == 2) {
			// std::cout << "SEND: " << send(fd, "GET / HTTP/1.1\r\nHost: a\r\n\r\n", 27, 0) << std::endl;
			// std::cout << "SEND: " << send(fd, "G", 1, 0) << std::endl;
			nextSend = true;
		}
		// buffer[bytes] = '\0';
		// std::cout << buffer << std::endl;
	}
	std::cout << "WAITED: " << std::time(0) - timer << std::endl;
	return (0);
}