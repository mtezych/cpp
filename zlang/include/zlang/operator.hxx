
#ifndef ZLANG_OPERATOR
#define ZLANG_OPERATOR

#include <string_view>

namespace zlang
{
    struct operator_
    {
        char symbol;
    };

    auto str (const operator_& op) -> std::string_view;
}

#endif
