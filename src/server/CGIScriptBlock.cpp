/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIScriptBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 04:10:14 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/24 05:01:29 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIScriptBlock.hpp"

CGIScriptBlock::CGIScriptBlock(void) :
cgiScripts(),
scriptAlias(),
scriptHandler()
{}

CGIScriptBlock::~CGIScriptBlock(void) {}

CGIScriptBlock::CGIScriptBlock(const CGIScriptBlock &obj) :
cgiScripts(obj.cgiScripts),
scriptAlias(obj.scriptAlias),
scriptHandler(obj.scriptHandler)
{}

CGIScriptBlock &CGIScriptBlock::operator=(const CGIScriptBlock &obj) {
    if (this == &obj)
        return (*this);
    this->cgiScripts = obj.cgiScripts;
    this->scriptAlias = obj.scriptAlias;
    this->scriptHandler = obj.scriptHandler;
    return (*this);
}
