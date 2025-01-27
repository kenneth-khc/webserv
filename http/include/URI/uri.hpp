/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:30:37 by cteoh             #+#    #+#             */
/*   Updated: 2025/01/27 01:26:13 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
# define URI_HPP

# include <string>

/***************/
/* General URI */
/***************/
bool	isAbsoluteURI(const std::string &line);

/**********/
/* Scheme */
/**********/
bool	isScheme(const std::string &line);

/*************/
/* Hier-Part */
/*************/
bool	isHierPart(const std::string &line);
bool	isAuthority(const std::string &line);
bool	isAbsolutePath(const std::string &line);
bool	isPort(const std::string &line);

/*********/
/* Query */
/*********/
bool	isQuery(const std::string &line);

/************/
/* Fragment */
/************/
bool	isFragment(const std::string &line);

#endif
