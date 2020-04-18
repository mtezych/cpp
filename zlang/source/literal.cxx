
#include <zlang/literal.hxx>

namespace zlang
{
    auto str (const literal& literal) -> std::string
    {
        return std::to_string(literal.number);
    }
}
