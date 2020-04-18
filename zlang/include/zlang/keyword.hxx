
#ifndef ZLANG_KEYWORD
#define ZLANG_KEYWORD

#include <string_view>

namespace zlang
{
    enum class keyword
    {
        EXTERN,
        DEF,
        IF,
        ELSE,
    };

    auto str (const keyword& keyword) -> std::string_view;
}

#endif
