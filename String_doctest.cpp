/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String_doctest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:55:12 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/07 17:14:57 by kecheong         ###   ########.fr       */
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

TEST_CASE("split()")
{
	String	s("Hi;How;Are;You");
	REQUIRE(s == "Hi;How;Are;You");

	SUBCASE("split by one existing delimiter")
	{
		vector<String> strings = s.split(";");
		CHECK(strings.size() == 4);
		CHECK(strings[0] == "Hi");
		CHECK(strings[1] == "How");
		CHECK(strings[2] == "Are");
		CHECK(strings[3] == "You");
	}

	SUBCASE("split by two existing delimiters")
	{
		vector<String>	strings = s.split(";H");
		CHECK(strings.size() == 4);
		CHECK(strings[0] == "i");
		CHECK(strings[1] == "ow");
		CHECK(strings[2] == "Are");
		CHECK(strings[3] == "You");
	}

	SUBCASE("split by nonexisting delimiters")
	{
		vector<String>	strings = s.split("zzz");
		CHECK(strings.size() == 1);
		CHECK(strings[0] == "Hi;How;Are;You");
	}

	SUBCASE("split by each individual character")
	{
		vector<String>	strings = s.split();
		CHECK(strings.size() == 14);
		for (size_t i = 0; i < s.size(); ++i)
		{
			CHECK(strings[i] == std::string(1, s[i]));
		}
	}

	SUBCASE("split empty string")
	{
		vector<String>	strings = String("").split("abc");
		CHECK(strings.size() == 0);
	}
}

TEST_CASE("trim()")
{
	const String	s("----token--");

	SUBCASE("trim with one matching character in the set")
	{
		String	trimmed = s.trim("-");
		CHECK(trimmed.size() == 5);
		CHECK(trimmed == "token");
	}

	SUBCASE("trim with two matching characters in the set")
	{
		String	trimmed = s.trim("-t");
		CHECK(trimmed.size() == 4);
		CHECK(trimmed == "oken");
	}

	SUBCASE("trim with three matching characters in the set")
	{
		String	trimmed = s.trim("t-n");
		CHECK(trimmed.size() == 3);
		CHECK(trimmed == "oke");
	}

	SUBCASE("trim only leading and trailing matches")
	{
		String	trimmed = s.trim("-oke");
		CHECK(trimmed.size() == 5);
		CHECK(trimmed == "token");
	}

	SUBCASE("trim with no matches in the set")
	{
		String	trimmed = s.trim("zzz");
		CHECK(trimmed.size() == s.size());
		CHECK(trimmed == s);
	}

	SUBCASE("trim empty string")
	{
		String	trimmed = String("").trim("-");
		CHECK(trimmed.size() == 0);
	}
}
