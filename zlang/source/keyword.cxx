
#include <zlang/keyword.hxx>

#include <cassert>

namespace zlang
{
    auto str (const keyword& keyword) -> std::string_view
    {
        switch (keyword)
        {
            case keyword::EXTERN :
            {
                return "extern";
            }
            case keyword::DEF :
            {
                return "def";
            }
            case keyword::IF :
            {
                return "if";
            }
            case keyword::ELSE :
            {
                return "else";
            }
            default :
            {
                assert(false);
                return "";
            }
        }
    }
}
