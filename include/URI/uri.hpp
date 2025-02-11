/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:30:37 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/11 22:38:53 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
# define URI_HPP

# include "String.hpp"

/***************/
/* General URI */
/***************/
bool	isAbsoluteURI(const String &line);

/**********/
/* Scheme */
/**********/
bool	isScheme(const String &line);

/*************/
/* Hier-Part */
/*************/
bool	isHierPart(const String &line);
bool	isAuthority(const String &line);
bool	isPort(const String &line);

/*********/
/* Query */
/*********/
bool	isQuery(const String &line);

/************/
/* Fragment */
/************/
bool	isFragment(const String &line);

#endif
