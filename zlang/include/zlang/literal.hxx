
#ifndef ZLANG_LITERAL
#define ZLANG_LITERAL

#include <string>

namespace zlang
{
    struct literal
    {
        float number; // value
    };

    auto str (const literal& literal) -> std::string;
}

#endif
