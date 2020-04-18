
// http://llvm.org/docs/tutorial/index.html

#include <zlang/lexer.hxx>

#include <iostream>

auto main () -> int
{
    const auto source_code = std::string_view
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
        "     cos(0.5)                                \n"
        "     sin(5.0)                                \n"
        "                                             \n"
    };

    auto lexer = zlang::lexer { source_code };

    while (const auto token = lexer.lex())
    {
        std::cout << zlang::str(*token) << std::endl;
    }

    return 0;
}
