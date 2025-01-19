/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 18:07:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/19 17:52:47 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debugUtils.hpp"

/* These are helper functions to convert a symbolic constant into a readable
 * form for debugging and visualization purposes */

/* They're not important, pls turn back */

#define TO_STRING(value) \
case value: \
	return #value;

const char*	dbg::familyToString(int ai_family)
{
	switch (ai_family)
	{
		TO_STRING(AF_UNSPEC);
		TO_STRING(AF_LOCAL | AF_UNIX | AF_FILE);
		TO_STRING(AF_INET);
		TO_STRING(AF_AX25);
		TO_STRING(AF_IPX);
		TO_STRING(AF_APPLETALK);
		TO_STRING(AF_NETROM);
		TO_STRING(AF_BRIDGE);
		TO_STRING(AF_ATMPVC);
		TO_STRING(AF_X25);
		TO_STRING(AF_INET6);
		TO_STRING(AF_ROSE);
		TO_STRING(AF_DECnet);
		TO_STRING(AF_NETBEUI);
		TO_STRING(AF_SECURITY);
		TO_STRING(AF_KEY);
		TO_STRING(AF_NETLINK | AF_ROUTE);
		TO_STRING(AF_PACKET);
		TO_STRING(AF_ASH);
		TO_STRING(AF_ECONET);
		TO_STRING(AF_ATMSVC);
		TO_STRING(AF_RDS);
		TO_STRING(AF_SNA);
		TO_STRING(AF_IRDA);
		TO_STRING(AF_PPPOX);
		TO_STRING(AF_WANPIPE);
		TO_STRING(AF_LLC);
		TO_STRING(AF_IB);
		TO_STRING(AF_MPLS);
		TO_STRING(AF_CAN);
		TO_STRING(AF_TIPC);
		TO_STRING(AF_BLUETOOTH);
		TO_STRING(AF_IUCV);
		TO_STRING(AF_RXRPC);
		TO_STRING(AF_ISDN);
		TO_STRING(AF_PHONET);
		TO_STRING(AF_IEEE802154);
		TO_STRING(AF_CAIF);
		TO_STRING(AF_ALG);
		TO_STRING(AF_NFC);
		TO_STRING(AF_VSOCK);
		TO_STRING(AF_KCM);
		TO_STRING(AF_QIPCRTR);
		TO_STRING(AF_SMC);
		TO_STRING(AF_XDP);
		TO_STRING(AF_MCTP);
		TO_STRING(AF_MAX);
		default: return "UNKNOWN FAMILY";
	}
}

const char*	dbg::protocolToString(int ai_protocol)
{
	switch (ai_protocol)
	{
		TO_STRING(IPPROTO_IP);
		TO_STRING(IPPROTO_ICMP);
		TO_STRING(IPPROTO_IGMP);
		TO_STRING(IPPROTO_IPIP);
		TO_STRING(IPPROTO_TCP);
		TO_STRING(IPPROTO_EGP);
		TO_STRING(IPPROTO_PUP);
		TO_STRING(IPPROTO_UDP);
		TO_STRING(IPPROTO_IDP);
		TO_STRING(IPPROTO_TP);
		TO_STRING(IPPROTO_DCCP);
		TO_STRING(IPPROTO_IPV6);
		TO_STRING(IPPROTO_RSVP);
		TO_STRING(IPPROTO_GRE);
		TO_STRING(IPPROTO_ESP);
		TO_STRING(IPPROTO_AH);
		TO_STRING(IPPROTO_MTP);
		TO_STRING(IPPROTO_BEETPH);
		TO_STRING(IPPROTO_ENCAP);
		TO_STRING(IPPROTO_PIM);
		TO_STRING(IPPROTO_COMP);
		TO_STRING(IPPROTO_SCTP);
		TO_STRING(IPPROTO_UDPLITE);
		TO_STRING(IPPROTO_MPLS);
		TO_STRING(IPPROTO_ETHERNET);
		TO_STRING(IPPROTO_RAW);
		TO_STRING(IPPROTO_MPTCP);
		default: return "UNKNOWN PROTOCOL";
	}
}

const char*	dbg::socktypeToString(int ai_socktype)
{
	switch (ai_socktype)
	{
		TO_STRING(SOCK_STREAM);
		TO_STRING(SOCK_DGRAM);
		TO_STRING(SOCK_RAW);
		TO_STRING(SOCK_RDM);
		TO_STRING(SOCK_SEQPACKET);
		TO_STRING(SOCK_DCCP);
		TO_STRING(SOCK_PACKET);
		default: return "UNKNOWN SOCKET TYPE";
	}
}

#define ADD_STRING(value) \
if (ai_flag & value) \
{ \
	str.append(#value); \
	str.append(" | "); \
} \

void	dbg::printFlagsToString(int ai_flag)
{
	std::string	str;
	ADD_STRING(AI_PASSIVE);
	ADD_STRING(AI_CANONNAME);
	ADD_STRING(AI_NUMERICHOST);
	ADD_STRING(AI_V4MAPPED);
	ADD_STRING(AI_ALL);
	ADD_STRING(AI_ADDRCONFIG);
# ifdef __USE_GNU
	ADD_STRING(AI_IDN);
	ADD_STRING(AI_CANONIDN);
# endif
	ADD_STRING(AI_NUMERICSERV);
	if (!str.empty())
		str.erase(str.size()-3);
	println(str);
}

void	dbg::printSocketAddr(sockaddr* sock)
{
	if (sock->sa_family == AF_INET)
	{
		sockaddr_in*	addr = reinterpret_cast<sockaddr_in*>(sock);
		char			ipv4[INET_ADDRSTRLEN];
		unsigned short	portNum = ntohs(addr->sin_port);
		inet_ntop(AF_INET, &addr->sin_addr, ipv4, INET_ADDRSTRLEN);
		std::cout << "ipv4         = " << ipv4 << '\n';
		std::cout << "port         = " << portNum << '\n';
	}
	else if (sock->sa_family == AF_INET6)
	{
		sockaddr_in6*	addr = reinterpret_cast<sockaddr_in6*>(sock);
		char			ipv6[INET6_ADDRSTRLEN];
		unsigned short	portNum = ntohs(addr->sin6_port);
		inet_ntop(AF_INET6, &addr->sin6_addr, ipv6, INET6_ADDRSTRLEN);
		std::cout << "ipv4         = " << ipv6 << '\n';
		std::cout << "port         = " << portNum << '\n';
	}
}

void	dbg::printAddrInfos(addrinfo* head)
{
	int	size = 0;
	for (addrinfo* it = head; it != NULL; it = it->ai_next)
		++size;
	std::cout << "Printing out " << size << " address information"
			  << (size>1 ? "s\n" : "\n");
	dbg::println("------------------------------------------------");
	for (; head != NULL; head = head->ai_next)
	{
		std::cout << "ai_flags     = "; dbg::printFlagsToString(head->ai_flags);
		std::cout << "ai_family    = " << dbg::familyToString(head->ai_family) << '\n';
		std::cout << "ai_socktype  = " << dbg::socktypeToString(head->ai_socktype) << '\n';
		std::cout << "ai_protocol  = " << dbg::protocolToString(head->ai_protocol) << '\n';
		dbg::printSocketAddr(head->ai_addr);
		if (head->ai_canonname)
			std::cout << "ai_canonname = " << head->ai_canonname << '\n';
		dbg::println("------------------------------------------------");
	}
	dbg::println("Printed out all addresses!");
}

void	dbg::prettyPrintReceived(char* buf, ssize_t bytes, int clientFD)
{
	std::cout << "Received " << bytes << " bytes from " << clientFD << ":\n";
	for (int i = 0; i < bytes; ++i)
		std::cout << '-';
	std::cout << '\n';
	std::cout << buf;
	for (int i = 0; i < bytes; ++i)
		std::cout << '-';
	std::cout << '\n';
}

