
// http://llvm.org/docs/tutorial/index.html

#include "Lexer.h"

#include <iostream>

int main()
{
	const auto code = std::string
	{
		"                                             \n"
		" extern sin(x)                               \n"
		" extern cos(x)                               \n"
		"                                             \n"
		" #                                           \n"
		" # returns n-th fibonacci number             \n"
		" #                                           \n"
		" def fibonacci(n)                            \n"
		"     if n < 3                                \n"
		"         1                                   \n"
		"     else                                    \n"
		"         fibonacci(n - 1) + fibonacci(n - 2) \n"
		"                                             \n"
		" def main()                                  \n"
		"     fibonacci(40)                           \n"
		"     cos(.5)                                 \n"
		"     sin(5.)                                 \n"
		"                                             \n"
	};

	auto lexer = zlang::Lexer { code };

	while (true)
	{
		const auto token = lexer.TokenizeNext();

		if (!token)
		{
			break;
		}

		std::cout << token << std::endl;
	}

	return 0;
}
