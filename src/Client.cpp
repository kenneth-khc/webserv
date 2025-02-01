/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 09:40:53 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/01 13:55:20 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"

// TODO: limit request size based on config file. now defaulted to 1 mb
const size_t	Client::MAX_REQUEST_SIZE(1 * 1024 * 1024);

Client::Client():
socketFD(),
address(),
addressLen(),
messageBuffer(),
message(),
request(),
requestLineFound(false),
headersFound(false),
hasBody(true),
bodyFound(false)
{
	messageBuffer.resize(1024);
}
