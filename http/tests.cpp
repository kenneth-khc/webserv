#include <string>
#include <vector>
#include <iostream>
#include "requestLine.hpp"

int	main(void) {
	std::vector<std::string>	tests;
	
	tests.push_back("GET /software/gnulib/manual.css HTTP/1.1");
	tests.push_back("GET /favicon.ico HTTP/1.1");
	tests.push_back("GET /software/gawk/manual/gawk_array-elements.png HTTP/1.1");

	for (std::size_t i = 0; i < tests.size(); i++) {
		std::cout << "\033[36m" << i << "\033[0m: ";
		if (isRequestLine(tests[i]) == true)
			std::cout << "✅ ";
		else
			std::cout << "❌ ";
		std::cout << "\033[33m" << tests[i] << "\033[0m" << std::endl;
	}
	return (0);
}
