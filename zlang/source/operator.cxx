
#include <zlang/operator.hxx>

namespace zlang
{
    auto str (const operator_& op) -> std::string_view
    {
        return std::string_view { &op.symbol, 1 };
    }
}
