
#include <zlang/identifier.hxx>

namespace zlang
{
    auto str (const identifier& identifier) -> std::string_view
    {
        return std::string_view { identifier.name };
    }
}
