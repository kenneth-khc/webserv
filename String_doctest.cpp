/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testString.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:55:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/07 16:55:18 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "String.hpp"

TEST_CASE("String")
{
	String	myString("Hello!");
	std::string	stdString("Hello!");

	REQUIRE(myString == stdString);
	REQUIRE(myString.size() == stdString.size());

	SUBCASE("Indexing")
	{
		CHECK(myString[0] == 'H');
		CHECK(myString[1] == 'e');
		CHECK(myString[5] == '!');
	}
	SUBCASE("Dereferencing")
	{
		CHECK(*myString == 'H');
		CHECK(*(myString+1) == 'e');
		CHECK(*(myString+5) == '!');
	}

	SUBCASE("Erasing")
	{
		CHECK(myString.consume());
		stdString.erase(0,1);
		CHECK(myString == stdString);
	}

	SUBCASE("Erasing until a !")
	{
		CHECK(myString.consumeUntil("!"));
		CHECK(myString == "!");
		CHECK(myString.size() == 1);
	}
}

TEST_CASE("String erasing")
{
	String	s("----abcde\r\n");

	SUBCASE("Erase until \r\n")
	{
		CHECK(s.consumeUntil("\r\n"));
		CHECK(s == "\r\n");
		CHECK(s.size() == 2);
	}

	SUBCASE("Erase until something that isn't there")
	{
		CHECK(s.consumeUntil("ababa") == false);
		CHECK(s == "----abcde\r\n");
	}

	SUBCASE("Erase if predicate is true")
	{
		CHECK(s.consumeIf('-'));
		CHECK(s == "---abcde\r\n");

		CHECK(s.consumeIf('Z') == false);
		CHECK(s == "---abcde\r\n");

		Predicate	p("-");
		CHECK(s.consumeIfUntil(p, "a"));
		CHECK(s == "abcde\r\n");
	}
}
