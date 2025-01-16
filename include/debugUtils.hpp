/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 18:12:37 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/16 19:15:33 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGUTILS_HPP
#define DEBUGUTILS_HPP

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define TO_STRING(value) \
case value: \
	return #value;

namespace DebuggingTools
{
	template<typename Type>	void	println(Type const&);
	const char*						familyToString(int);
	const char*						protocolToString(int);
	const char*						socktypeToString(int);
	void							printFlagsToString(int);
	void							printAddrInfos(addrinfo* head);
	void							printSocketAddr(sockaddr* sock);
};

namespace dbg = DebuggingTools;

template <typename Type>
void	dbg::println(Type const& t)
{
	std::cout << t << '\n';
}

#endif
