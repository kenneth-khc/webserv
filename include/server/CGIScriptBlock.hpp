/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIScriptBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 04:05:08 by cteoh             #+#    #+#             */
/*   Updated: 2025/11/24 04:23:17 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGISCRIPTBLOCK_HPP
# define CGISCRIPTBLOCK_HPP

# include "Optional.hpp"
# include "String.hpp"

# include <vector>
# include <utility>

class CGIScriptBlock {
	public:
		CGIScriptBlock(void);
		~CGIScriptBlock(void);
		CGIScriptBlock(const CGIScriptBlock &obj);
		CGIScriptBlock	&operator=(const CGIScriptBlock &obj);
		std::vector<String>						cgiScripts;
		Optional< std::pair<String, String> >	scriptAlias;
		Optional< std::pair<String, String> >	scriptHandler;
};

#endif