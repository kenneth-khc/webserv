/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInput.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:13:25 by cteoh             #+#    #+#             */
/*   Updated: 2025/04/03 03:55:21 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "Driver.hpp"
#include "CGI.hpp"
#include "CGIInput.hpp"

CGIInput::CGIInput(
Driver &driver,
CGI &cgi,
int fd) :
	cgi(cgi),
	epollFD(driver.epollFD),
	cgis(driver.cgis),
	fd(fd),
	inputLength(0),
	contentLength(cgi.request.find< Optional<String::size_type> >("Content-Length").value)
{
	epoll_event ev;
	ev.events = EPOLLOUT;
	fcntl(fd, F_SETFL, O_NONBLOCK);
	ev.data.fd = fd;
	epoll_ctl(driver.epollFD, EPOLL_CTL_ADD, fd, &ev);
}

CGIInput::~CGIInput(void) {
	if (this->fd != -1)
		this->close();
}

void	CGIInput::feed(std::set<CGI*>& activeCGIs) {
	ssize_t	bytes = 0;

	if (this->inputLength < this->contentLength && cgi.request.messageBody.length() > 0) {
		ssize_t	bytesToWrite = this->contentLength - this->inputLength;

		bytes = write(this->fd, cgi.request.messageBody.c_str(), bytesToWrite);
		if (bytes > 0) {
			activeCGIs.insert(&cgi);
			this->inputLength += bytes;
			cgi.request.messageBody.erase(0, bytes);
		}
	}
	if (this->inputLength == this->contentLength)
		this->close();
}

void	CGIInput::close(void) {
	epoll_ctl(epollFD, EPOLL_CTL_DEL, this->fd, 0);
	::close(this->fd);
	cgis.erase(this->fd);
	this->fd = -1;
}
