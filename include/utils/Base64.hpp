/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:40:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/13 23:03:20 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_HPP
# define BASE64_HPP

# include "String.hpp"

# if UCHAR_MAX == 255
#  define ucharInRange(character) true
# else
#  define ucharInRange(character) (character <= 255)
# endif

namespace Base64 {
	String	encode(const String &str);
	bool	decode(const String &str, String &output);
}

#endif
