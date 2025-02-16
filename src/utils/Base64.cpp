/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 03:36:07 by cteoh             #+#    #+#             */
/*   Updated: 2025/02/13 23:07:07 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "Base64.hpp"

String	Base64::encode(const String &str) {
	static const std::string	base64Characters = \
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string	encodedStr;
	std::size_t	originalLength = str.length();
	std::size_t	encodedLength = str.length();
	std::size_t	index;
	const char	*strPtr = str.c_str();

	while (encodedLength % 4 != 0)
		encodedLength++;

	while (originalLength != 0 && encodedLength != 0) {
		index = (static_cast<unsigned char>(strPtr[0]) >> 2) & 63;
		encodedStr.push_back(base64Characters[index]);

		encodedLength--;
		if (encodedLength == 0)
			break ;

		index = (static_cast<unsigned char>(strPtr[0]) << 4);
		originalLength--;
		if (originalLength > 0)
			index += (static_cast<unsigned char>(strPtr[1]) >> 4);
		else
			index += 0;
		index &= 63;
		encodedStr.push_back(base64Characters[index]);

		encodedLength--;
		if (encodedLength == 0)
			break ;

		if (originalLength > 0) {
			index = (static_cast<unsigned char>(strPtr[1]) << 2);
			originalLength--;
			if (originalLength > 0)
				index += (static_cast<unsigned char>(strPtr[2]) >> 6);
			else
				index += 0;
			index &= 63;
			encodedStr.push_back(base64Characters[index]);
		}
		else
			encodedStr.push_back('=');

		encodedLength--;
		if (encodedLength == 0)
			break ;

		if (originalLength > 0) {
			index = static_cast<unsigned char>(strPtr[2]) & 63;
			encodedStr.push_back(base64Characters[index]);
		}
		else
			encodedStr.push_back('=');

		encodedLength--;
		if (encodedLength == 0)
			break ;

		if (originalLength > 0)
			originalLength--;
		if (originalLength > 0)
			strPtr += 3;
	}
	return (encodedStr);
}

char	base64ToASCII(unsigned char character) {
	if (character >= 'A' && character <= 'Z')
		return (character - 65);
	else if (character >= 'a' && character <= 'z')
		return (character - 71);
	else if (character >= '0' && character <= '9')
		return (character + 4);
	else if (character == '+')
		return (62);
	else if (character == '/')
		return (63);
	else
		return (-1);
}

bool	isBase64Character(char character) {
	unsigned char	unsignedCharacter = character;

		return (ucharInRange(unsignedCharacter) && (0 <= base64ToASCII(unsignedCharacter)));
}

bool	Base64::decode(const String &str, String &output) {
	std::size_t	encodedLength = str.length();
	const char	*strPtr = str.c_str();

	while (encodedLength >= 2) {
		if (isBase64Character(strPtr[0]) == false || isBase64Character(strPtr[1]) == false)
			break ;

		output += ((base64ToASCII(strPtr[0]) << 2) | (base64ToASCII(strPtr[1]) >> 4));

		if (encodedLength == 2)
			break ;

		if (strPtr[2] == '=') {
			if (encodedLength != 4)
				break ;
			if (strPtr[3] != '=')
				break ;
		}
		else {
			if (isBase64Character(strPtr[2]) == false)
				break ;

			output += ((base64ToASCII(strPtr[1]) << 4) & 240) | (base64ToASCII(strPtr[2]) >> 2);

			if (encodedLength == 3)
				break ;

			if (strPtr[3] == '=') {
				if (encodedLength != 4)
					break ;
			}
			else {
				if (isBase64Character(strPtr[3]) == false)
					break ;

				output += ((base64ToASCII(strPtr[2]) << 6) & 192) | base64ToASCII(strPtr[3]);
			}
		}
		strPtr += 4;
		encodedLength -= 4;
	}

	if (encodedLength != 0)
		return (false);
	return (true);
}
