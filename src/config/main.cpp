/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:38:00 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/12 22:23:17 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "RDP.hpp"
#include "Token.hpp"
#include "Lexer.hpp"

int	main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "where's the config file you bozo?\n";
		return 1;
	}

	Lexer	lexer(argv[1]);
	RDP		parser(argv[1]);

	if (parser.configFile)
	{
		Token	t = lexer.getNextToken();
		while (t != Token::END_OF_FILE)
		{
			std::cout << "Lex: " << t.lexeme << '\n';
			t = lexer.getNextToken();
		}
	}
}
