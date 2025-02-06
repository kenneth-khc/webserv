/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testString.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:55:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/06 19:09:05 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "String.hpp"

int	main()
{
	String	s("Hello World");

	s.consumeUntil("rld");
	std::cout << s.match(String("ld"), 1) << '\n';

}
