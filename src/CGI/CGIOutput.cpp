/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIOutput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:37:04 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/20 03:12:59 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <sys/epoll.h>
#include <fcntl.h>
#include "ErrorCode.hpp"
#include "connection.hpp"
#include "Driver.hpp"
#include "CGI.hpp"
#include "CGIOutput.hpp"
#include "CGIOutputState.hpp"
#include "CGIHeadersState.hpp"

CGIOutput::CGIOutput(CGI &cgi, int epollFD, std::map<int, CGI*> &cgis, int fd) :
	cgi(cgi),
	epollFD(epollFD),
	cgis(cgis),
	fd(fd),
	pipeSize(fcntl(fd, F_GETPIPE_SZ)),
	output(),
	state(0)
{
	epoll_event	ev;
	ev.events = EPOLLIN;
	fcntl(fd, F_SETFL, O_NONBLOCK);
	ev.data.fd = fd;
	epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &ev);
}

CGIOutput::~CGIOutput(void) {
	if (this->fd != -1)
		this->close();
	if (this->state != 0)
		delete this->state;
}

void	CGIOutput::fetch(std::set<Timer*>& activeTimers) {
	ssize_t				bytes = -1;
	std::vector<char>	buffer(this->pipeSize);

	bytes = read(this->fd, &buffer[0], this->pipeSize);

	if (bytes > 0)
		this->output.append(&buffer[0], bytes);
	if (bytes >= 0) {
		activeTimers.insert(cgi.timer);
		this->processState(bytes);
	}
	if (bytes == 0) {
		constructConnectionHeader(cgi.request, cgi.response);
		cgi.response.insert("Date", Time::printHTTPDate());
		cgi.response.processStage |= Response::DONE;
		this->close();
	}
}

void	CGIOutput::processState(const ssize_t &bytes) {
	if (this->state == 0)
		this->state = new CGIHeadersState();

	CGIOutputState	*newState = this->state->parse(cgi.response, this->output, bytes);

	if (newState != this->state) {
		delete this->state;
		this->state = newState;
	}
}

void	CGIOutput::close(void) {
	epoll_ctl(epollFD, EPOLL_CTL_DEL, this->fd, 0);
	::close(this->fd);
	cgis.erase(this->fd);
	this->fd = -1;
}
