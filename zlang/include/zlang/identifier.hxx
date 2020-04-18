
#ifndef ZLANG_IDENTIFIER
#define ZLANG_IDENTIFIER

#include <string>
#include <string_view>

namespace zlang
{
    struct identifier
    {
        std::string name;
    };

    auto str (const identifier& identifier) -> std::string_view;
}

#endif
