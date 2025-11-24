/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIScriptBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 04:10:14 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/24 07:41:21 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Defaults.hpp"
#include "CGIScriptBlock.hpp"

CGIScriptBlock::CGIScriptBlock(void) :
cgiScripts(),
scriptAlias(),
scriptHandler(),
allowedMethods(),
clientMaxBodySize()
{}

CGIScriptBlock::~CGIScriptBlock(void) {}

CGIScriptBlock::CGIScriptBlock(const CGIScriptBlock &obj) :
cgiScripts(obj.cgiScripts),
scriptAlias(obj.scriptAlias),
scriptHandler(obj.scriptHandler),
allowedMethods(obj.allowedMethods),
clientMaxBodySize(obj.clientMaxBodySize)
{}

CGIScriptBlock &CGIScriptBlock::operator=(const CGIScriptBlock &obj) {
    if (this == &obj)
        return (*this);
    this->cgiScripts = obj.cgiScripts;
    this->scriptAlias = obj.scriptAlias;
    this->scriptHandler = obj.scriptHandler;
    this->allowedMethods = obj.allowedMethods;
    this->clientMaxBodySize = obj.clientMaxBodySize;
    return (*this);
}

CGIScriptBlock::CGIScriptBlock(const Directive &block) :
cgiScripts(),
scriptAlias(),
scriptHandler(),
allowedMethods(block.getInherited("allow_method", Defaults::ALLOW_METHODS)),
clientMaxBodySize(block.getInherited("client_max_body_size", Defaults::CLIENT_MAX_BODY_SIZE).toSize())
{
    const std::vector<Parameter>&	extensions = block.getParameters();

    std::vector<String>	trimmed;
    for (size_t i = 0; i < extensions.size(); ++i)
    {
        trimmed.push_back(extensions[i].value.substr(1));
    }
    this->cgiScripts = trimmed;

    Optional<Directive*>	scriptAlias = block.getDirective("script_alias");

    if (scriptAlias.exists)
    {
        const std::vector<Parameter>&	paths = scriptAlias.value->getParameters();
        this->scriptAlias = makeOptional(std::make_pair(paths[0].value, paths[1].value));
    }

    Optional<Directive*>	scriptHandler = block.getDirective("script_handler");

    if (scriptHandler.exists)
    {
        const std::vector<Parameter>&	paths = scriptHandler.value->getParameters();
        this->scriptHandler = makeOptional(std::make_pair(paths[0].value, paths[1].value));
    }
}
