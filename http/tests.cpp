#include <string>
#include <vector>
#include <iostream>
#include "requestLine.hpp"
#include "fields.hpp"

int	main(void) {
	std::vector<std::string>	tests;
	std::vector<std::string>	results;
	
	tests.push_back("GET /software/gnulib/manual.css HTTP/1.1");
	results.push_back("✅");
	tests.push_back("GET /favicon.ico HTTP/1.1");
	results.push_back("✅");
	tests.push_back("GET /software/gawk/manual/gawk_array-elements.png HTTP/1.1");
	results.push_back("✅");
	tests.push_back("POST /~smith/home.html HTTP/1.1");
	results.push_back("✅");
	tests.push_back("POST /\%7Esmith/home.html HTTP/1.1");
	results.push_back("✅");

	tests.push_back("POST /\%7esmith/home.html HTTP/1.1");
	results.push_back("✅");
	tests.push_back("POST /kenneth-khc/webserv/projects?query=is\%3Aopen HTTP/1.1");
	results.push_back("✅");
	tests.push_back("GET /software/gnulib/manual.css");
	results.push_back("❌");
	tests.push_back("GET HTTP/1.1");
	results.push_back("❌");
	tests.push_back("GET /software one/gnulib/manual.css HTTP/1.1");
	results.push_back("❌");

	tests.push_back("/software/gnulib/manual.css HTTP/1.1");
	results.push_back("❌");
	tests.push_back("G@T  /software/gnulib/manual.css  HTTP/1.1");
	results.push_back("❌");
	tests.push_back("GET /путь/страница.html HTTP/1.1");
	results.push_back("❌");

	std::cout << "\033[32m" << "Request Line:" << "\033[0m" << std::endl;
	for (std::size_t i = 0; i < tests.size(); i++) {
		std::cout << "\033[36m" << i + 1 << "\033[0m: " << results[i] << '/';
		if (isRequestLine(tests[i]) == true)
			std::cout << "✅ ";
		else
			std::cout << "❌ ";
		std::cout << "\033[33m" << tests[i] << "\033[0m" << std::endl;
	}
	std::cout << std::endl;
	tests.erase(tests.begin(), tests.end());
	results.erase(results.begin(), results.end());

	tests.push_back("Host: en.cppreference.com");
	results.push_back("✅");
	tests.push_back("User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0");
	results.push_back("✅");
	tests.push_back("Accept: text/css,*/*;q=0.1");
	results.push_back("✅");
	tests.push_back("Accept-Language: en-US,en;q=0.5");
	results.push_back("✅");
	tests.push_back("Accept-Encoding: gzip, deflate, br, zstd");
	results.push_back("✅");

	tests.push_back("Accept-Encoding gzip, deflate, br, zstd");
	results.push_back("❌");
	tests.push_back(": gzip, deflate, br, zstd");
	results.push_back("❌");
	tests.push_back("Accept Encoding: gzip, deflate, br, zstd");
	results.push_back("❌");
	tests.push_back("Accept-Encøding: gzip, deflate, br, zstd");
	results.push_back("❌");
	tests.push_back("Accept-Encoding : gzip, deflate, br, zstd");
	results.push_back("❌");

	tests.push_back("Accept-Encoding:");
	results.push_back("✅");

	std::cout << "\033[32m" << "Headers:" << "\033[0m" << std::endl;
	for (std::size_t i = 0; i < tests.size(); i++) {
		std::cout << "\033[36m" << i + 1 << "\033[0m: " << results[i] << '/';
		if (isFieldLine(tests[i]) == true)
			std::cout << "✅ ";
		else
			std::cout << "❌ ";
		std::cout << "\033[33m" << tests[i] << "\033[0m" << std::endl;
	}
	std::cout << std::endl;
	return (0);
}
